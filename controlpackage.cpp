#ifndef CONTROLPACKAGE
#define CONTROLPACKAGE
struct controlpackage{
  char command; // Command
  char axis_x; // forwards - backwards
  char axis_y; // up -down
  char axis_z; // left - right
  char axis_r; // Rotation
  char cam_x;  // Pan
  char cam_y;  // Tilt
  unsigned long timestamp;
};
#endif