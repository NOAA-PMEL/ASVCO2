#include "statistics.h"

void STATS_Clear(sStats_F_t *s)
{

  s->numSamples = 0u;
  s->sum = 0.0f;
  s->M2 = 0.0f;
  s->M2_old = 0.0f;
  s->mean = 0.0f;
  s->mean_old = 0.0f;
  s->std = 0.0f;
  s->min = 0.0f;
  s->max = 0.0f;

  return;
}

void STATS_Clear_I16(sStats_I16_t *s)
{
  s->M2 = 0;
  s->M2_old = 0;
  s->max = 0;
  s->mean = 0;
  s->mean_old = 0;
  s->min = 0;
  s->numSamples = 0;
  s->std = 0;
  s->sum = 0;
}

void STATS_Add(sStats_F_t *var, float newSample)
{
  /** Based on Knuth */
  var->numSamples++;

  if (var->numSamples == 1)
  {
    var->mean_old = newSample;
    var->mean = newSample;
    var->M2_old = 0.0;
  }
  else
  {
    var->mean = var->mean_old + (newSample - var->mean_old) / var->numSamples;
    var->M2 = var->M2_old + (newSample - var->mean_old) * (newSample - var->mean);

    var->mean_old = var->mean;
    var->M2_old = var->M2;
  }

  if (newSample < var->min)
  {
    var->min = newSample;
  }

  if (newSample > var->max)
  {
    var->max = newSample;
  }
}

void STATS_Compute(sStats_F_t *s)
{
  float var = s->M2 / (s->numSamples);

  s->std = sqrt(var);
}

void STATS_AddSample_I16(sStats_I16_t *var, int16_t newSample)
{

  var->numSamples++;
  if (var->numSamples == 1)
  {
    var->mean_old = newSample;
    var->mean = newSample;
    var->M2 = 0;
    var->M2_old = 0;
  }
  else
  {
    var->mean = var->mean_old + (newSample - var->mean_old) / var->numSamples;
    var->M2 = var->M2_old + (newSample - var->mean_old) * (newSample - var->mean);

    var->mean_old = var->mean;
    var->M2_old = var->M2;
  }

  if (newSample < var->min)
  {
    var->min = newSample;
  }

  if (newSample > var->max)
  {
    var->max = newSample;
  }
}

void STATS_Compute_I16(sStats_I16_t *s)
{
  s->std = (int16_t)sqrt(s->M2);
}

void STATS_Clear_I32(sStats_I32_t *s)
{

  s->numSamples = 0;
  s->sum = 0;
  s->M2 = 0;
  s->mean = 0;
  s->std = 0;
  s->min = 0x7FFFFFFF;
  s->max = 0;
}

void STATS_AddSample_I32(sStats_I32_t *s, int32_t newSample)
{
  int64_t delta = newSample - s->mean;
  s->numSamples++;
  s->mean += (delta / s->numSamples);
  s->M2 += (delta * (newSample - s->mean));
  if (s->max < newSample)
    ;
}