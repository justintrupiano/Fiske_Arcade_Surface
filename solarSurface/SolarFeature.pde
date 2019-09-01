class SolarFeature{
  PShape  outsideShape;
  PShape  insideShape;

  PVector pos = new PVector(0, 0);
  PVector vel = new PVector(0, 0);
  PVector acc = new PVector(0, 0);

  int     destArch = round(random(0, 11));

  int     type;
  int     age = 0;                          //// CURRENT LIFETIME OF THIS FEATURE (IN FPS)
  int     outsideOpacity = 64;
  int     insideOpacity = 200;

  float   lifeSpan = 10 * frameRate;       //// NUM OF SECONDS * FPS TO KEEP FEATURE AT MAX OPACITY (ASSUMING ~60FPS)
  float   currentSize = 0;
  float   maxSize = random(canvasWidth*0.01, canvasWidth*0.2);  //// MAX SIZE OF FEATURE (10% OF CANVAS WIDTH)
  float   perlinSeed = random(100);
  float   increment;
  float   flux = 0.75;

  boolean maxed = false;

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
    perlinSeed += 0.025;                                  //// SPEED OF FLUX WITHIN FEATURE
    outsideShape = createShape();
    outsideShape.beginShape();
    insideShape = createShape();
    insideShape.beginShape();
    increment = random(maxSize*0.001, maxSize*0.03);

    for (float v = 0; v < TWO_PI; v+=0.025){
        float xoff = map(cos(v)+perlinSeed, -1, 1, 1, 3); //// JAGGEDNESS OF EDGES (HIGHER = MORE JAGGED)
        float yoff = map(sin(v)+perlinSeed, -1, 1, 1, 3); //// JAGGEDNESS OF EDGES (HIGHER = MORE JAGGED)

        //// MOVING THROUGH NOISE SPACE:
        float n = map((float) (noise .eval(xoff, yoff)), -1, 1, 0, 1);
        float r = map(n, -1, 1, currentSize - flux, currentSize);

        float vX = r * cos(v);
        float vY = r * sin(v);

        outsideShape.vertex(vX, vY);
        insideShape.vertex(vX, vY);

    }

    outsideShape.endShape(CLOSE);
    insideShape.endShape(CLOSE);
    insideShape.scale(0.5);

    outsideShape.setFill(color(46, 1, 4, outsideOpacity));
    insideShape.setFill(color(46, 1, 4, insideOpacity));

  }



  void showSpots(){
    // pushMatrix();
      // translate(pos.x, pos.y);  //// PLACE UNDER MOUSE (OR WHEREVER pos.x/pos.y HAS BEEN SET)
      shape(outsideShape, pos.x, pos.y);
      shape(insideShape, pos.x, pos.y);
    // popMatrix();



  }

  int count = 0;

  void moveSpot(){
    //


    //   ///// MOVE TOWARD DEST
    PVector dest = new PVector();
    dest = arches[destArch].copy();
    // if (dist(pos.x, pos.y, dest.x, dest.y) < 2 && age < lifeSpan){
    if (dist(pos.x, pos.y, dest.x, dest.y) < 2){ //// DISTANCE FROM GOAL TO TRIGGER EFFECT

        // println("Arch: " + destArch);
        // println("Itsy: " + floor(destArch/2));
        // println(ports.length/2);
        if (floor(destArch/2) <= ports.length/2){ //// ONLY TRIGGER EFFECT IF THE ITSY IS IN THE PORTS ARRAY
          // ports[floor(destArch/2)].write(chosenViz);


          // println("Arch: " + destArch);
          println("Viz: " + chosenViz);
          // println(floor(destArch/2));
        }


    }


    dest.sub(pos);
    dest.setMag(0.001);
    acc = dest;

    // vel = new PVector(0, 0);
    // acc = new PVector(0, 0);
    acc.add(PVector.random2D().mult(0.005));
    vel.add(acc);
    pos.add(vel);





  }


  void update(){
    makeSpotShape(); //// MAKE NEW SHAPE WITH SLIGHTLY DIFFERENT VERTS (CREATES THE FLUX EFFECT)
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
