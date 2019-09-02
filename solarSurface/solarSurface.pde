import processing.serial.*;

Serial[] ports;

String dataDir    = "../data/";
OPC opc;

PImage solarSurface;
PImage spotColor;

int canvasWidth   = 55; //// 55
int canvasHeight  = 91; //// 19
int maxNumFeatures = 50;

float noiseIncrement = 0.25;  /// CHANGE SIZE OF GRANUAL
float zoff = 0.0;
float xoffStart = 90.0;

char[] archVisualizations = {'a', 'b', 'c', 'd'};

OpenSimplexNoise noise;

// PImage plageColor;

ArrayList<SolarFeature> solarFeatures;

PVector[] arches = new PVector[12];
String[] itsybitsies;


void settings(){
  size(canvasWidth, canvasHeight);
}

void setup(){

  makeItsyArray();

  opc = new OPC(this, "127.0.0.1", 7890);  // Connect to the local instance of fcserver
  opc.ledGrid(0, 54, 90, width/2, height/2, width/width, height/height, 0, false, true); // Create LED Grid

  noStroke();

  noise         = new OpenSimplexNoise();
  solarFeatures = new ArrayList<SolarFeature>();
  solarSurface  = loadImage(dataDir + "solarSurface.png");
  spotColor     = loadImage(dataDir + "sunspot.png");
  // plageColor    = loadImage(dataDir + "plage.png");


  // itsybitsies[0]
  arches[0] = new PVector(0, 7);
  arches[1] = new PVector(width-1, 8);

  // itsybitsies[1]
  arches[2] = new PVector(0, 11);
  arches[3] = new PVector(width-1, 23);

  // itsybitsies[2]
  arches[4] = new PVector(0, 27);
  arches[5] = new PVector(width-1, 30);

  // itsybitsies[3]
  arches[6] = new PVector(0, 64);
  arches[7] = new PVector(width-1, 48);

  // itsybitsies[4]
  arches[8] = new PVector(0, 43);
  arches[9] = new PVector(width-1, 60);

  // itsybitsies[5]
  arches[10] = new PVector(0, 77);
  arches[11] = new PVector(width-1, 82);
}



void draw() {
  drawSurface();
  drawFeatures();
}


void mousePressed(){
    if (solarFeatures.size() < maxNumFeatures){
      //// 0 == SPOT  /   1 == PLAGE
      // solarFeatures.add(new SolarFeature(mouseX, mouseY, 0));
      solarFeatures.add(new SolarFeature(mouseX, mouseY, 0));
  }
}


void drawFeatures(){
  if (solarFeatures.size() > 0){
    for (int i = 0; i < solarFeatures.size(); i++){
      if (solarFeatures.get(i).type == 0) {
        solarFeatures.get(i).show();
        solarFeatures.get(i).update();
      }
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
  }
}


void serialEvent(Serial p){
  int received = p.read();
  if (received == '1' && solarFeatures.size() < maxNumFeatures){
      //// 9 + (INDEX * 18)
      for (int i = 0; i < itsybitsies.length-1; i++){
        if (ports[i] == p){
          //// 9 + (INDEX * 18)
          solarFeatures.add(new SolarFeature(round((width/2)+random(-5, 5)), 9 + (i*18), 0));
          break;
        }
      }
  }
}


void showArchLocations(){
  for (int a = 0; a < arches.length; a++){
    fill(map(a, 0, arches.length, 0, 255));
    ellipse(arches[a].x, arches[a].y, 10, 10);
  }
}
