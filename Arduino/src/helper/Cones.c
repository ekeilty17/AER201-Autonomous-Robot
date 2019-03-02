#define HOLE 1
#define LEFT_CRACK 2
#define CENTER_CRACK 3
#define RIGHT_CRACK 4

// Cone configurations
//    Cone positions can range from 0cm to 17cm
void cone_positions(float* out, int obstruction) {
  switch(obstruction) {
    case HOLE:
      out[0] = 8.5;   //8.5 - 17.5
      out[1] = 8.5;   //no cone
      break;
    case LEFT_CRACK:
      out[0] = 0;     //0 - 9
      out[1] = 10;    //10 - 19
      break;
    case CENTER_CRACK:
      out[0] = 3;     //3 - 12
      out[1] = 13;    //13 - 22
      break;
    case RIGHT_CRACK:
      out[0] = 7;     //7 - 16
      out[1] = 17;    //17 - 26
      break;
  }
}
