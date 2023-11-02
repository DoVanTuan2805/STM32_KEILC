
#include "simpleKalmanFilter.h"

void KalmanInit(float mea_e, float est_e, float q)
{
	dataKalman._err_measure=mea_e;
	dataKalman._err_estimate=est_e;
	dataKalman._q = q;
	dataKalman._current_estimate = 0;
	dataKalman._last_estimate = 0;
	dataKalman._kalman_gain = 0;
}
float updateEstimate(float mea)
{
  dataKalman._kalman_gain = dataKalman._err_estimate/(dataKalman._err_estimate + dataKalman._err_measure);
  dataKalman._current_estimate = dataKalman._last_estimate + dataKalman._kalman_gain * (mea - dataKalman._last_estimate);
  dataKalman._err_estimate =  (1.0 - dataKalman._kalman_gain)*dataKalman._err_estimate + fabs(dataKalman._last_estimate-dataKalman._current_estimate)*dataKalman._q;
  dataKalman._last_estimate=dataKalman._current_estimate;

  return dataKalman._current_estimate;
}

void setMeasurementError(float mea_e)
{
  dataKalman._err_measure=mea_e;
}

void setEstimateError(float est_e)
{
  dataKalman._err_estimate=est_e;
}

void setProcessNoise(float q)
{
  dataKalman._q=q;
}

float getKalmanGain() {
  return dataKalman._kalman_gain;
}

float getEstimateError() {
  return dataKalman._err_estimate;
}
