#ifndef SimpleKalmanFilter_h
#define SimpleKalmanFilter_h

#include <math.h>
typedef struct 
{
  float _err_measure;
  float _err_estimate;
  float _q;
  float _current_estimate;
  float _last_estimate;
  float _kalman_gain;
}DataSimpleKalman_t;
static DataSimpleKalman_t dataKalman;


void KalmanInit(float mea_e, float est_e, float q);
float updateEstimate(float mea);
void setMeasurementError(float mea_e);
void setEstimateError(float est_e);
void setProcessNoise(float q);
float getKalmanGain();
float getEstimateError();

#endif