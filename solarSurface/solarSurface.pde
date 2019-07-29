String dataDir    = "../data/";
OPC opc;

int canvasWidth   = 55;
int canvasHeight  = 19;


float noiseIncrement = 0.25;  /// CHANGE SIZE OF GRANUAL
float zoff = 0.0;
float xoffStart = 90.0;

OpenSimplexNoise noise;

PImage solarSurface;
PImage sunspotColor;

ArrayList<Sunspot> sunspots;

void settings(){
  size(canvasWidth, canvasHeight);

}

void setup(){
  // blendMode(REPLACE);  //// FOR BLENDING OF THE SUNSPOTS WITH THE SURFACE

  opc = new OPC(this, "127.0.0.1", 7890);  // Connect to the local instance of fcserver
  opc.ledGrid(0, 54, 18, width/2, height/2, width/width, height/height, 0, false, false); // Create LED Grid
  
  noStroke();
  noFill();

  noise         = new OpenSimplexNoise();
  sunspots      = new ArrayList<Sunspot>();
  solarSurface  = loadImage(dataDir + "solarSurface.png");
  sunspotColor  = loadImage(dataDir + "sunspot.png");
}

void draw() {
  drawSurface();
  drawSunspots();
}


void mousePressed(){
  int spotDensity = 4; //// MAKING MULTIPLE SPOTS ON TOP OF EACH OTHER MAKES A NICE FADING EFFECT
    for (int s = 0; s < spotDensity; s++){
      sunspots.add(new Sunspot(mouseX, mouseY));
  }
}


void drawSunspots(){
  if (sunspots.size() > 0){
    for (int i = 0; i < sunspots.size(); i++){
      sunspots.get(i).show();
      sunspots.get(i).update();
      if (sunspots.get(i).currentSize <= 0){
        sunspots.remove(i);
      }
    }
  }
}


void drawSurface(){
  loadPixels();
  float xoff = xoffStart;
  xoffStart -= 0.01;              /// ADJUST SPEED OF LEFT-RIGHT FLOW
  zoff += 0.025;                  /// ADJUST SPEED OF 3D FLOW

  for (int x = 0; x < width; x++) {
    xoff += noiseIncrement;
    float yoff = 0.0;
    for (int y = 0; y < height; y++) {
      yoff += noiseIncrement;

      float n = (float) noise.eval(xoff, yoff, zoff);
      int colorPos = round(map(n, -1, 1, 0, solarSurface.width));
      pixels[x+y*width] = color(solarSurface.get(colorPos, 0));
    }
  }
  updatePixels();
}
