class SolarFeature{
  PShape  outsideShape;
  PShape  insideShape;

  PVector pos = new PVector(0, 0);
  PVector vel = new PVector(0, 0);
  PVector acc = new PVector(0, 0);

  int     destArch = round(random(0, 11));

  int     type;

  //// CURRENT LIFETIME OF THIS FEATURE (IN FPS)
  int     age = 0;
  int     outsideOpacity = 64;
  int     insideOpacity = 178;

//// NUM OF SECONDS * FPS TO KEEP FEATURE AT MAX OPACITY (ASSUMING ~60FPS)
  float   lifeSpan = 5 * frameRate;
  float   currentSize = 0;
  float   maxSize = random(canvasWidth*0.01, canvasWidth*0.2);  //// MAX SIZE OF FEATURE (10% OF CANVAS WIDTH)
  float   perlinSeed = random(100);
  float   increment;
  float   flux = 0.5;

  boolean maxed = false;
  boolean sentFlare = false;

  color   featureColor;

  char chosenViz = archVisualizations[round(random(0, archVisualizations.length-1))];

  SolarFeature(int x, int y, int type){
    pos.x = x;
    pos.y = y;

    if (type == 0){
      makeSpotShape();
    }

  }

  void show(){
    showSpots();
  }

  void makeSpotShape(){
    //// SPEED OF FLUX WITHIN FEATURE
    perlinSeed += 0.025;

    //// SHAPE CONSISTS OF TWO RANDOMLY GENERATED SHAPES
    outsideShape = createShape();
    outsideShape.beginShape();
    insideShape = createShape();
    insideShape.beginShape();

    /*
    / INCREMENT IS THE RATE AT WHICH A FEATURE GROWS AND SHRINKS
    / MAX SIZE IS ASSIGNED FOR EACH FEATURE AS A RANDOM NUMBER canvasWidth*0.01, canvasWidth*0.2
    */
    increment = random(maxSize*0.001, maxSize*0.03);

    //// SMALLER v INCREMENT == MORE VERTICES IN CIRCLE
    for (float v = 0; v < TWO_PI; v+=0.025){
        float xoff = map(cos(v)+perlinSeed, -1, 1, 1, 5);
        float yoff = map(sin(v)+perlinSeed, -1, 1, 1, 5);

        //// MOVING THROUGH NOISE-SPACE:
        //// HIGHER n == MORE JAGGED SHAPE
        float n = map((float)(noise.eval(xoff, yoff)), -1, 1, 0, 4);
        float r = map(n, -1, 1, currentSize - flux, currentSize);
        float vX = r * cos(v);
        float vY = r * sin(v);

        //// ADD NEW VERTEX WITH NOISE-SPACE VALUES
        outsideShape.vertex(vX, vY);
        insideShape.vertex(vX, vY);

    }


    outsideShape.endShape(CLOSE);
    insideShape.endShape(CLOSE);
    insideShape.scale(0.75);

    outsideShape.setFill(color(46, 1, 4, outsideOpacity));
    insideShape.setFill(color(46, 1, 4, insideOpacity));

  }



  void showSpots(){
    //// MAKE SHAPE USING THE DEFINED VERTICIES
    shape(outsideShape, pos.x, pos.y);
    shape(insideShape, pos.x, pos.y);
  }



  void moveSpot(){

    ///// MOVE TOWARD DEST
    PVector dest = new PVector();
    dest = arches[destArch].copy();

    //// CHECK IF SPOT IS NEAR ITS DESTINATION ARCH
    if (dist(pos.x, pos.y, dest.x, dest.y) < 2 && age < lifeSpan && !sentFlare){

      //// ONLY TRIGGER EFFECT IF THE ITSY IS IN THE PORTS ARRAY
      if (floor(destArch/4) <= ports.length/2){
            ports[floor(destArch/2)].write(chosenViz);

      //// sentFlare ENSURES SPOTS WILL ONLY SEND 1 FLARE
  	  sentFlare = true;

    	}
    }



    /*
      THE SPOTS MOVE IN A DIRECTION WHICH IS SLIGHTLY RANDOM...
      ...AND SLIGHTLY GUIDED IN THE DIRECTION OF THEIR SPECIFIC GOAL.
    */
    dest.sub(pos);
    dest.setMag(0.001);
    acc = dest;

    //// RANDOM MOVEMENT OF SPOTS
    acc.add(PVector.random2D().mult(0.0025));
    vel.add(acc);
    pos.add(vel);
  }


  void update(){

    //// MAKE NEW SHAPE WITH SLIGHTLY DIFFERENT VERTS (CREATES THE FLUX EFFECT)
    makeSpotShape();
    moveSpot();

    if (currentSize < maxSize && maxed == false){
      currentSize += increment;
    }
    else{
      maxed = true;
    }

    if(maxed){
      age += 1;
    }

    if (age > lifeSpan){
      currentSize -= increment;
    }
  }
}
