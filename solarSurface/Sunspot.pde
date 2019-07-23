class Sunspot{
  PShape  spotShape;

  int     xPos;
  int     yPos;
  int     spotAge = 0;                          //// CURRENT LIFETIME OF THIS SPOT (IN FPS)

  float     spotLifeSpan = 5 * frameRate;       //// NUM OF SECONDS * FPS TO KEEP SPOT AT MAX OPACITY (ASSUMING ~60FPS)
  float   currentSize = 0;
  float   maxSize = random(canvasWidth*0.001, canvasWidth*0.1);  //// MAX SIZE OF SUNSPOT (10% OF CANVAS WIDTH)
  float   perlinSeed = random(1)*100;
  float   increment = maxSize*0.005;
  float   flux = 10;

  Sunspot(int x, int y){
    xPos = x;
    yPos = y;
    makeSpotShape();
  }

  void makeSpotShape(){
    perlinSeed += 0.025;                                  //// SPEED OF FLUX WITHIN SPOT
    spotShape = createShape();
    spotShape.beginShape();

    for (float v = 0; v < TWO_PI; v+=0.025){
        float xoff = map(cos(v)+perlinSeed, -1, 1, 1, 3); //// JAGGEDNESS OF EDGES (HIGHER = MORE JAGGED)
        float yoff = map(sin(v)+perlinSeed, -1, 1, 1, 3); //// JAGGEDNESS OF EDGES (HIGHER = MORE JAGGED)

        //// MOVING THROUGH NOISE SPACE:
        float n = map((float) (noise.eval(xoff, yoff)), -1, 1, 0, 1);
        float r = map(n, 0, 1, constrain(currentSize - flux, 0, currentSize), currentSize);

        float vX = r * cos(v);
        float vY = r * sin(v);
        spotShape.vertex(vX, vY);
    }
    spotShape.endShape(CLOSE);
  }

  void show(){
    pushMatrix();
      fill(color(46, 1, 4, 64));              //// color(46, 1, 4) = DARK RED-ISH
      translate(xPos, yPos);                  //// PLACE UNDER MOUSE (OR WHEREVER xPos/yPos HAS BEEN SET)
      shape(spotShape);
      spotShape.scale(2);                     //// DOUBLE SHAPE SIZE
      shape(spotShape);
      spotShape.scale(0.5);                   //// RESIZE BACK TO NORMAL //// SIMPLER THAN COPYING CUSTOM SHAPES
    popMatrix();
  }

  void update(){
    makeSpotShape(); //// MAKE NEW SHAPE WITH SLIGHTLY DIFFERENT VERTS (CREATES THE FLUX EFFECT)

    if (currentSize >= maxSize){
      increment = 0;
      spotAge += 1;
      if (spotAge >= spotLifeSpan){
        increment = -(maxSize*0.005);
      }
    }

    currentSize += increment;

  }
}
