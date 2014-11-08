#ifndef CONTROLPACKAGE
#define CONTROLPACKAGE
struct controlpackage{
  char state;
  //ei komentoa = 0, laita paalle = 1, sammuta = 2, kamera paalle = 3,  kamera pois = 4,
  char axis_x; // forwards - backwards
  char axis_y; // up -down
  char axis_z; // left - right
  char axis_r; // Rotation
  char cam_x;  // Pan
  char cam_y;  // Tilt
  //kummallekin akselille yksi byte, 0: paikallaan, 1 ja 2 eri suunnat
  //Optimoi myöh. yhteen byteen
  //LISÄÄ TIMESTAMP
};
#endif
