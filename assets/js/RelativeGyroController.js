class RelativeGyroController {

  constructor(camera) {
    this.camera = camera;

    this.enabled = false;
    this.firstRead = true;
    this.activationPose = "upright"; // upright | flatUp | flatDown

    this.initialCameraQuat = null;
    this.sensorZeroQuat = null;
    this.currentDeviceQuat = null;

    this.smoothing = 0.15;

    this._onOrientation = this._onOrientation.bind(this);
  }

  enable() {
    if (this.enabled) return;

    this.enabled = true;
    this.firstRead = true;
    this.initialCameraQuat = this.camera.rotationQuaternion.clone();

    window.addEventListener("deviceorientation", this._onOrientation, true);
  }

  disable() {
    this.enabled = false;
    window.removeEventListener("deviceorientation", this._onOrientation, true);
  }

  _detectActivationPose(betaDeg, gammaDeg) {
    const isFlatLike = Math.abs(gammaDeg) < 35;
    const isFlatUp   = isFlatLike && (Math.abs(betaDeg) < 35);
    const isFlatDown = isFlatLike && (Math.abs(Math.abs(betaDeg) - 180) < 35);

    if (isFlatUp) return "flatUp";
    if (isFlatDown) return "flatDown";
    return "upright";
  }

  _onOrientation(event) {
    if (!this.enabled) return;

    const alpha = BABYLON.Tools.ToRadians(event.alpha || 0);
    const beta  = BABYLON.Tools.ToRadians(event.beta  || 0);
    const gamma = BABYLON.Tools.ToRadians(event.gamma || 0);

    if (this.firstRead) {
      this.activationPose = this._detectActivationPose(event.beta || 0, event.gamma || 0);
    }

    let yaw = alpha;
    let pitch = beta;
    let roll = gamma;

    if (this.activationPose === "flatUp" || this.activationPose === "flatDown") {
      const tmp = yaw;
      yaw = roll;
      roll = tmp;

      if (this.activationPose === "flatUp") {
        roll = -roll;
      }

      if (this.activationPose === "flatDown") {
        roll = -roll;
      }
    }

    const qYaw   = BABYLON.Quaternion.RotationAxis(BABYLON.Axis.Y, yaw);
    const qPitch = BABYLON.Quaternion.RotationAxis(BABYLON.Axis.X, pitch);
    const qRoll  = BABYLON.Quaternion.RotationAxis(BABYLON.Axis.Z, roll);

    let deviceQuat = qYaw.multiply(qPitch).multiply(qRoll);

    if (this.activationPose === "flatDown") {
      const qFlip = BABYLON.Quaternion.RotationAxis(BABYLON.Axis.X, Math.PI);
      deviceQuat.multiplyInPlace(qFlip);
    }

    if (this.firstRead) {
      this.sensorZeroQuat = deviceQuat.clone().invert();
      this.firstRead = false;
    }

    const relative = deviceQuat.multiply(this.sensorZeroQuat);
    const finalQuat = relative.multiply(this.initialCameraQuat);

    BABYLON.Quaternion.SlerpToRef(
      this.camera.rotationQuaternion,
      finalQuat,
      this.smoothing,
      this.camera.rotationQuaternion
    );
  }
}