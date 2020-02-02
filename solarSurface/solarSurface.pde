import processing.serial.*;

Serial[] ports;

String dataDir    = "../data/";
OPC opc;

PImage solarSurface;
PImage spotColor;

int canvasWidth   = 55; //// 55
int canvasHeight  = 91; //// 91
int maxNumFeatures = 50;

float noiseIncrement = 0.25;  /// CHANGE SIZE OF GRANUAL
float zoff = 0.0;
float xoffStart = 90.0;

char[] archVisualizations = {'a'}; //// ADD MORE FOR DIFFERENT EFFECTS --- THESE ARE CHOSEN AT RANDOM IN THE


OpenSimplexNoise noise;

// PImage plageColor;

ArrayList<SolarFeature> solarFeatures;

PVector[] arches;
String[] itsybitsies;


void settings(){
  size(canvasWidth, canvasHeight);

}

void setup(){

  noStroke();
  blendMode(MULTIPLY);

  //// OPC SERVER NEEDS TO RUN FOR FADECANDIES TO WORK
  opc = new OPC(this, "127.0.0.1", 7890);  // Connect to the local instance of fcserver
  opc.ledGrid(0, 54, 90, width/2, height/2, width/width, height/height, 0, false, true); // Create LED Grid

  //// BUILD ARRAY OF MOUNTED ARDUINOS --- GETS LIST FROM serials TEXT FILE
  makeItsyArray();

  noise         = new OpenSimplexNoise();
  solarFeatures = new ArrayList<SolarFeature>();
  solarSurface  = loadImage(dataDir + "solarSurface.png");
  spotColor     = loadImage(dataDir + "sunspot.png");
  // plageColor    = loadImage(dataDir + "plage.png"); // IN CASE OF PLAGE


 //// THESE ARE THE LOCATIONS OF EACH ARCH AND THIER CORRESPONDING ARDUINOS
  arches = new PVector[12];
  // itsybitsies[0]
  arches[0] = new PVector(0, 12);
  arches[1] = new PVector(width-1, 7);

  // itsybitsies[1]
  arches[2] = new PVector(0, 25);
  arches[3] = new PVector(width-1, 41);

  // itsybitsies[2]
  arches[4] = new PVector(0, 46);
  arches[5] = new PVector(width-1, 29);

  // itsybitsies[3]
  arches[6] = new PVector(0, 62);
  arches[7] = new PVector(width-1, 59);

  // itsybitsies[4]
  arches[8] = new PVector(0, 78);
  arches[9] = new PVector(width-1, 66);

  // itsybitsies[5]
  arches[10] = new PVector(0, 82);
  arches[11] = new PVector(width-1, 81);
  /////////////////////////////////////////////////////////////////////////

}



void draw() {
  drawSurface();
  drawFeatures();

  //// Draw spots if not in use.
  if (solarFeatures.size() == 0){
    showArchLocations(); /// SHOULD BE CALL MAKE-SPOT-AT-ARCH
  }
  if (second() % 10 == 0){
      for (int i = 0; i < 3; i++){
        solarFeatures.add(new SolarFeature(round(width/2 + random(-5, 5)), round(random(0, height)), 0, true));
      }
  }



}


void mousePressed(){
  ports[3].write('a');
    //if (solarFeatures.size() < maxNumFeatures){
      //// 0 == SPOT  /   1 == PLAGE
      // solarFeatures.add(new SolarFeature(mouseX, mouseY, 0));
      //solarFeatures.add(new SolarFeature(mouseX, mouseY, 0));
  //}
}


void drawFeatures(){
  if (solarFeatures.size() > 0){
    for (int i = 0; i < solarFeatures.size(); i++){
      // if (solarFeatures.get(i).type == 0) {
        solarFeatures.get(i).show();
        solarFeatures.get(i).update();
      // }
  }
    for (int i = 0; i < solarFeatures.size(); i++){
      if (solarFeatures.get(i).currentSize < 0){
        solarFeatures.remove(i);  //// REMOVE OLDER SUN SPOTS
      }
    }
  }
}


void drawSurface(){
  loadPixels();
  float xoff = xoffStart;
  xoffStart -= 0.01;              /// ADJUST SPEED OF LEFT-RIGHT FLOW
  zoff += 0.05;                  /// ADJUST SPEED OF 3D FLOW

  for (int x = 0; x < width; x++) {
    xoff += noiseIncrement;
    float yoff = 0.0;
    for (int y = 0; y < height; y++) {
      yoff += noiseIncrement;

      float n = (float) noise.eval(xoff, yoff, zoff);

      int colorPos;
      PImage colorField;
      colorField = solarSurface;

      colorPos = round(map(n, -1, 1, 0, colorField.width));
      pixels[x+y*width] = color(colorField.get(colorPos, 0));
    }
  }
  updatePixels();
}


void makeItsyArray(){
  itsybitsies = loadStrings( dataDir + "panel_order");

  ports = new Serial[itsybitsies.length];
  for (int i = 0; i < ports.length; i++){
    ports[i] = new Serial(this, itsybitsies[i], 9600);
	println(i);
  }
}


void serialEvent(Serial p){
int serialStream = p.read();
// if (serialStream == 49){
  println(serialStream);
// }
  int received = serialStream;
  if (received == 49 && solarFeatures.size() < maxNumFeatures && random(100) > 75.8){
      //// 9 + (INDEX * 18)
      for (int i = 0; i < ports.length; i++){
        if (ports[i] == p){
          //// 9 + (INDEX * 18)
          solarFeatures.add(new SolarFeature(round((width/2)+random(-5, 5)), 9 + (i*18), 0, true));
          received = 0;
          break;
        }
      }
  }
}


void showArchLocations(){
  for (int a = 0; a < arches.length; a++){
    solarFeatures.add(new SolarFeature((int)arches[a].x, (int)arches[a].y, 0, false));

  }
}
