/*******************************************************************************
 * 
 * Written by Lauren Turnbow
 * 
 * This code calculates the time constant of the Langmuir Probes after they are 
 * biased. Given the number of data points, an array of the time variables, and
 * array of the voltage. The time will be given, the voltage will be read in
 * from the PIC from the Langmuir probes after it leaks off after being biased
 * 
 ******************************************************************************/

double exponentialCalculation(int numDataPoints, double time[], double voltage[])
{
  // double time[] is x coordinate, double voltage[] is y coordinate
  int i = 0;                                        // Loop counter
  double b, timeConstant;                			// Constants
  double x2y = 0;                                   // Terms from equations
  double ylny = 0;
  double xy = 0;
  double xylny = 0;
  double y = 0;
  
  // Calculating each sum term
  for (i = 0; i < numDataPoints; ++i)
  {
    x2y += time[i] * time[i] * voltage[i];
  }
  
  for (i = 0; i < numDataPoints; ++i)
  {
    ylny += voltage[i] * log(voltage[i]);
  }
  
  for (i = 0; i < numDataPoints; ++i)
  {
    xy += time[i] * voltage[i];
  }
  
  for (i = 0; i < numDataPoints; ++i)
  {
    xylny += time[i] * voltage[i] * log(voltage[i]);
  }
  
  for (i = 0; i < numDataPoints; ++i)
  {
    y += voltage[i];
  }
  
  // Calculating constants
  b = (y * xylny - xy * ylny) / (y * x2y - xy * xy);
  timeConstant = -1.0 / b;
  
  return timeConstant;
}