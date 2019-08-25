String dataDir    = "../data/";
OPC opc;

PImage solarSurface;
PImage spotColor;

int canvasWidth   = 55; //// 55
int canvasHeight  = 91; //// 19
int maxNumFeatures = 100;

float noiseIncrement = 0.25;  /// CHANGE SIZE OF GRANUAL
float zoff = 0.0;
float xoffStart = 90.0;

OpenSimplexNoise noise;

// PImage plageColor;

ArrayList<SolarFeature> solarFeatures;

PVector[] arches = new PVector[12];


void settings(){
  size(canvasWidth, canvasHeight);
}

void setup(){
  // blendMode(DARKEST);  //// FOR BLENDING OF THE FEATURES WITH THE SURFACE
  // frameRate(5);

  opc = new OPC(this, "127.0.0.1", 7890);  // Connect to the local instance of fcserver
  opc.ledGrid(0, 54, 90, width/2, height/2, width/width, height/height, 0, false, false); // Create LED Grid

  noStroke();

  noise         = new OpenSimplexNoise();
  solarFeatures = new ArrayList<SolarFeature>();
  solarSurface  = loadImage(dataDir + "solarSurface.png");
  spotColor     = loadImage(dataDir + "sunspot.png");
  // plageColor    = loadImage(dataDir + "plage.png");


  ///////////// HARD CODE POS OF ARCHES /////////////
  arches[0] = new PVector(0, 0);
  arches[1] = new PVector(0, 55);

  arches[2] = new PVector(18, 0);
  arches[3] = new PVector(18,55);

  arches[4] = new PVector(36, 0);
  arches[5] = new PVector(36,55);

  arches[6] = new PVector(54, 0);
  arches[7] = new PVector(54,55);

  arches[8] = new PVector(72, 0);
  arches[9] = new PVector(72,55);

  arches[10] = new PVector(91, 0);
  arches[11] = new PVector(91,55);
  ///////////////////////////////////



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
    // else if (solarFeatures.get(i).type == 1) {
    //
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

      // if (dist(x,y, x, height/2) < 10){
      //   colorField = plageColor;
      // }
      // else{
        colorField = solarSurface;
      // }

      colorPos = round(map(n, -1, 1, 0, colorField.width));
      pixels[x+y*width] = color(colorField.get(colorPos, 0));


    }
  }
  updatePixels();
}
