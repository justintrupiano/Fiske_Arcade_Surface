class SolarFeature{
  PShape  featureShape;
  PVector pos = new PVector(0,0);

  int     type;
  int     age = 0;                          //// CURRENT LIFETIME OF THIS FEATURE (IN FPS)

  float   lifeSpan = 5 * frameRate;       //// NUM OF SECONDS * FPS TO KEEP FEATURE AT MAX OPACITY (ASSUMING ~60FPS)
  float   currentSize = 0;
  float   maxSize = random(canvasWidth*0.001, canvasWidth*0.1);  //// MAX SIZE OF FEATURE (10% OF CANVAS WIDTH)
  float   perlinSeed = random(100);
  float   increment = maxSize*0.005;
  float   flux = 10;

  color   featureColor;

  SolarFeature(int x, int y, int type){
    pos.x = x;
    pos.y = y;

    if (type == 0){
      makeSpotShape();
    }
  }

  void makeSpotShape(){
    perlinSeed += 0.025;                                  //// SPEED OF FLUX WITHIN FEATURE
    featureShape = createShape();
    featureShape.beginShape();

    for (float v = 0; v < TWO_PI; v+=0.025){
        float xoff = map(cos(v)+perlinSeed, -1, 1, 1, 3); //// JAGGEDNESS OF EDGES (HIGHER = MORE JAGGED)
        float yoff = map(sin(v)+perlinSeed, -1, 1, 1, 3); //// JAGGEDNESS OF EDGES (HIGHER = MORE JAGGED)

        //// MOVING THROUGH NOISE SPACE:
        float n = map((float) (noise.eval(xoff, yoff)), -1, 1, 0, 1);
        float r = map(n, 0, 1, constrain(currentSize - flux, 0, currentSize), currentSize);

        float vX = r * cos(v);
        float vY = r * sin(v);
        featureShape.vertex(vX, vY);
    }
    featureShape.endShape(CLOSE);
  }

  void show(){
    showSpots();
  }

  void showSpots(){
    pushMatrix();
      fill(color(46, 1, 4, 64));              //// color(46, 1, 4) = DARK RED-ISH
      // fill(color(250, 234, 206, 64));            //// color(250, 234, 206) = BRIGHT YELLOW-ISH
      translate(pos.x, pos.y);                     //// PLACE UNDER MOUSE (OR WHEREVER pos.x/pos.y HAS BEEN SET)
      shape(featureShape);
      featureShape.scale(2);                     //// DOUBLE SHAPE SIZE
      shape(featureShape);
      featureShape.scale(0.5);                   //// RESIZE BACK TO NORMAL //// SIMPLER THAN COPYING CUSTOM SHAPES
    popMatrix();
  }

  void update(){
    makeSpotShape(); //// MAKE NEW SHAPE WITH SLIGHTLY DIFFERENT VERTS (CREATES THE FLUX EFFECT)

    if (currentSize >= maxSize){
      increment = 0;
      age += 1;
      if (age >= lifeSpan){
        increment = -(maxSize*0.005);
      }
    }

    currentSize += increment;

  }
}
