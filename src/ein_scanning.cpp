WORD(DownsampleIrScan)
CODE(1048690) // numlock + r
virtual void execute() {
  // replace unsampled regions with the lowest z reading, highest reading in those maps because they are inverted
  // 
  double highestReading = -VERYBIGNUMBER;
  double highestEpsilonMassReading = -VERYBIGNUMBER;
  double readingFloor = -1;
  for (int rx = 0; rx < rmWidth; rx++) {
    for (int ry = 0; ry < rmWidth; ry++) {
      for (int rrx = rx*10; rrx < (rx+1)*10; rrx++) {
        for (int rry = ry*10; rry < (ry+1)*10; rry++) {
          if (hiRangeMapMass[rrx + rry*hrmWidth] > 0.0) {
            //if ((hiRangeMap[rrx + rry*hrmWidth] > highestReading) && (hiRangeMap[rrx + rry*hrmWidth] >= readingFloor))
            if ((hiRangeMap[rrx + rry*hrmWidth] > highestEpsilonMassReading) && (hiRangeMapMass[rrx + rry*hrmWidth] > EPSILON))
              highestEpsilonMassReading = hiRangeMap[rrx + rry*hrmWidth];

            if ((hiRangeMap[rrx + rry*hrmWidth] > highestReading) && (hiRangeMapMass[rrx + rry*hrmWidth] > 0))
              highestReading = hiRangeMap[rrx + rry*hrmWidth];
          }
        }
      }
    }
  }


  if (highestReading <= -VERYBIGNUMBER) {
    highestReading = 0;
  }

	
  for (int rx = 0; rx < rmWidth; rx++) {
    for (int ry = 0; ry < rmWidth; ry++) {
      double thisSum = 0;
      double numSamples = 0;
      for (int rrx = rx*10; rrx < (rx+1)*10; rrx++) {
        for (int rry = ry*10; rry < (ry+1)*10; rry++) {
          numSamples += 1.0;
          if (hiRangeMapMass[rrx + rry*hrmWidth] > 0.0) {
            thisSum += hiRangeMap[rrx + rry*hrmWidth];
          } else {
            thisSum += 0;
          }
        }
      }
      rangeMapReg1[rx + ry*rmWidth] = thisSum/numSamples;
    }
  }
}
END_WORD




WORD(ScanObject)
CODE(196708)     // capslock + D
virtual void execute() {
  cout << "ENTERING WHOLE FOODS VIDEO MAIN." << endl;
  cout << "Program will pause shortly. Please adjust height for bounding box servo before unpausing." << endl;
  cout << "Program will pause a second time. Please adjust height for IR scan before unpausing." << endl;
  cout << "Program will pause a third time. Please remove any applied contrast agents." << endl;

  eepReg2 = rssPose;
  eepReg4 = rssPose;

  // so that closest servoing doesn't go into gradient servoing.
  targetClass = -1;


  // this automatically changes learning mode
          
  pilot_call_stack.push_back(1245242); // begin bounding box learning

  pilot_call_stack.push_back(1245248); // change to height 1
  pilot_call_stack.push_back(1048625); // change to first gear

  // set target class to the lastLabelLearned 
  pilot_call_stack.push_back(1179730);

  pilot_call_stack.push_back(131142); // reinitialize and retrain everything

  // set lastLabelLearned
  pilot_call_stack.push_back(1179732);

  pilot_call_stack.push_back(131143); // 72 way scan
  pilot_call_stack.push_back(131154); // w1 wait until at current position
  pilot_call_stack.push_back(131143); // 72 way scan
  pilot_call_stack.push_back(131154); // w1 wait until at current position

  // this is a good time to remove a contrast agent
  //pilot_call_stack.push_back('Y'); // pause stack execution
  //pushCopies(1245308, 15); // beep
	  
  { // do density and gradient, save gradient, do medium scan in two directions, save range map
    pushSpeedSign(MOVE_FAST);
    pilot_call_stack.push_back(196705); // save current depth map to current class
    pilot_call_stack.push_back(1048622); // neutral scan 
    pilot_call_stack.push_back('Y'); // pause stack execution
    pushCopies(1245308, 15); // beep
    pushSpeedSign(MOVE_FAST);

    pilot_call_stack.push_back(1245248); // change to height 1

    {
      pilot_call_stack.push_back(196730); // save aerial gradient map if there is only one blue box
      pushCopies(131121, densityIterationsForGradientServo); // density
      pilot_call_stack.push_back(262237); // reset aerialGradientTemporalFrameAverage
      pushCopies(131121, 1); // density
      pilot_call_stack.push_back(131153); // vision cycle
      pilot_call_stack.push_back(131154); // w1 wait until at current position
      pilot_call_stack.push_back(1245220); // change to height 3
    }
    {
      pilot_call_stack.push_back(196730); // save aerial gradient map if there is only one blue box
      pushCopies(131121, densityIterationsForGradientServo); // density
      pilot_call_stack.push_back(262237); // reset aerialGradientTemporalFrameAverage
      pushCopies(131121, 1); // density
      pilot_call_stack.push_back(131153); // vision cycle
      pilot_call_stack.push_back(131154); // w1 wait until at current position
      pilot_call_stack.push_back(1245219); // change to height 2
    }
    {
      pilot_call_stack.push_back(196730); // save aerial gradient map if there is only one blue box
      pushCopies(131121, densityIterationsForGradientServo); // density
      pilot_call_stack.push_back(262237); // reset aerialGradientTemporalFrameAverage
      pushCopies(131121, 1); // density
      pilot_call_stack.push_back(131153); // vision cycle
      pilot_call_stack.push_back(131154); // w1 wait until at current position
      pilot_call_stack.push_back(1245248); // change to height 1
    }
    {
      pilot_call_stack.push_back(196730); // save aerial gradient map if there is only one blue box
      pushCopies(131121, densityIterationsForGradientServo); // density
      pilot_call_stack.push_back(262237); // reset aerialGradientTemporalFrameAverage
      pushCopies(131121, 1); // density
      pilot_call_stack.push_back(131153); // vision cycle
      pilot_call_stack.push_back(131154); // w1 wait until at current position
      pilot_call_stack.push_back(1245217); // change to height 0
    }
  }

  // ATTN 3
  // start NO bag routine
  pilot_call_stack.push_back(196720); //  make a new class

  pilot_call_stack.push_back(131139); // synchronic servo don't take closest
  pilot_call_stack.push_back(131156); // synchronic servo
  pilot_call_stack.push_back(196707); // synchronic servo take closest
  pilot_call_stack.push_back(131153); // vision cycle

  pilot_call_stack.push_back('Y'); // pause stack execution
  pushCopies(1245308, 15); // beep

  pilot_call_stack.push_back(131154); // w1 wait until at current position
  pilot_call_stack.push_back(1048625); // change to first gear
  pilot_call_stack.push_back(1245219); // change to height 2
  pushSpeedSign(MOVE_FAST);
  pilot_call_stack.push_back(196672); // go to wholeFoodsCounter1

  pilot_call_stack.push_back(1179735); // change to counter table
  pilot_call_stack.push_back(1048625); // change to first gear
  pilot_call_stack.push_back('k'); // open gripper
}
END_WORD





WORD(PrepareForSearch)
CODE(1114150)     // numlock + &
virtual void execute() {
  currentEEPose.px = rmcX + drX;
  currentEEPose.py = rmcY + drY;
}
END_WORD

 

WORD(TurnOnRecordRangeMap)
CODE(1048683) 
virtual void execute() {
  recordRangeMap = 1;
}
END_WORD

WORD(InitDepthScan)
CODE(1048695) // numlock + w
virtual void execute() {
  cout << "Set rmcX and rmcY. Resetting maps. " << rmcX << " " << trueEEPose.position.x << endl;
  rmcX = trueEEPose.position.x;
  rmcY = trueEEPose.position.y;
  rmcZ = trueEEPose.position.z - eeRange;
  for (int rx = 0; rx < rmWidth; rx++) {
    for (int ry = 0; ry < rmWidth; ry++) {
      rangeMap[rx + ry*rmWidth] = 0;
      rangeMapReg1[rx + ry*rmWidth] = 0;
      // ATTN 17
      //rangeMapReg2[rx + ry*rmWidth] = 0;
      rangeMapMass[rx + ry*rmWidth] = 0;
      rangeMapAccumulator[rx + ry*rmWidth] = 0;
    }
  }
  {
    cv::Scalar backColor(128,0,0);
    cv::Point outTop = cv::Point(0,0);
    cv::Point outBot = cv::Point(rmiWidth,rmiHeight);
    Mat vCrop = rangemapImage(cv::Rect(outTop.x, outTop.y, outBot.x-outTop.x, outBot.y-outTop.y));
    vCrop = backColor;
  }
  for (int rx = 0; rx < hrmWidth; rx++) {
    for (int ry = 0; ry < hrmWidth; ry++) {
      hiRangeMap[rx + ry*hrmWidth] = 0;
      hiRangeMapReg1[rx + ry*hrmWidth] = 0;
      hiRangeMapReg2[rx + ry*hrmWidth] = 0;
      hiRangeMapMass[rx + ry*hrmWidth] = 0;
      hiRangeMapAccumulator[rx + ry*hrmWidth] = 0;
    }
  }
  {
    cv::Scalar backColor(128,0,0);
    cv::Point outTop = cv::Point(0,0);
    cv::Point outBot = cv::Point(hrmiWidth,hrmiHeight);
    Mat vCrop = hiRangemapImage(cv::Rect(outTop.x, outTop.y, outBot.x-outTop.x, outBot.y-outTop.y));
    vCrop = backColor;
  }
  for (int h = 0; h < hrmWidth; h++) {
    for (int i = 0; i < hrmWidth; i++) {
      hiColorRangeMapMass[h + i*hrmWidth] = 0;
      for (int j = 0; j < 3; j++) {
        hiColorRangeMapAccumulator[h + i*hrmWidth + j*hrmWidth*hrmWidth] = 0;
      }
    }
  }
  for (int pz = 0; pz < vmWidth; pz++) {
    for (int py = 0; py < vmWidth; py++) {
      for (int px = 0; px < vmWidth; px++) {
        volumeMap[px + py*vmWidth + pz*vmWidth*vmWidth] = 0;
        volumeMapAccumulator[px + py*vmWidth + pz*vmWidth*vmWidth] = 0;
        volumeMapMass[px + py*vmWidth + pz*vmWidth*vmWidth] = 0;
        vmColorRangeMapMass[px + py*vmWidth + pz*vmWidth*vmWidth] = 0;
        for (int pc = 0; pc < 3; pc++) {
          vmColorRangeMapAccumulator[px + py*vmWidth + pz*vmWidth*vmWidth + pc*vmWidth*vmWidth*vmWidth] = 0;
        }
      }
    }
  }
  {
    cv::Scalar backColor(128,0,0);
    cv::Point outTop = cv::Point(0,0);
    cv::Point outBot = cv::Point(hiColorRangemapImage.cols,hiColorRangemapImage.rows);
    Mat vCrop = hiColorRangemapImage(cv::Rect(outTop.x, outTop.y, outBot.x-outTop.x, outBot.y-outTop.y));
    vCrop = backColor;
  }
}
END_WORD





WORD(NeutralScan)
CODE(1048622) // numlock + .
virtual void execute() {
  cout << "Entering neutral scan." << endl;
  double lineSpeed = MOVE_FAST;//MOVE_MEDIUM;//MOVE_FAST;
  double betweenSpeed = MOVE_FAST;//MOVE_MEDIUM;//MOVE_FAST;
  ////pushCopies('e', 3);
  ////pushCopies('q', 10);
  ////scanYdirection(lineSpeed, betweenSpeed); // load scan program
  //scanXdirection(lineSpeed, betweenSpeed); // load scan program
  ////pushCopies('q', 3);
  ////pushCopies('e', 10);

  //pilot_call_stack.push_back(1048684); // turn off scanning

  scanXdirection(lineSpeed, betweenSpeed); // load scan program
  pilot_call_stack.push_back(1114150); // prepare for search

  pushCopies('q',4);
  pushCopies('a',6);

  pilot_call_stack.push_back(1048683); // turn on scanning
  pushNoOps(60);
  pilot_call_stack.push_back(1114155); // rotate gear

  pilot_call_stack.push_back(1114183); // full render
  pilot_call_stack.push_back(1048679); // render reticle
  pilot_call_stack.push_back(1048625); // change to first gear
  pilot_call_stack.push_back(1048673); // render register 1
  pilot_call_stack.push_back(1048690); // load map to register 1
  {
    pilot_call_stack.push_back(1048678); // target best grasp
    pilot_call_stack.push_back(131154); // w1 wait until at current position
    pilot_call_stack.push_back(1048625); // change to first gear
  }
  pilot_call_stack.push_back(1048630); // find best grasp

  scanXdirection(lineSpeed, betweenSpeed); // load scan program
  pilot_call_stack.push_back(1114150); // prepare for search

  pilot_call_stack.push_back(1048683); // turn on scanning
  pilot_call_stack.push_back(1048695); // clear scan history
}
END_WORD


WORD(SaveAerialGradientMap)
CODE(196730)      // capslock + Z
virtual void execute() {
  Size sz = objectViewerImage.size();
  int imW = sz.width;
  int imH = sz.height;
        
  cout << "save aerial gradient ";
  if ((focusedClass > -1) && (frameGraySobel.rows >1) && (frameGraySobel.cols > 1)) {
    string thisLabelName = focusedClassLabel;

    char buf[1000];
    string dirToMakePath = data_directory + "/" + thisLabelName + "/aerialGradient/";
    string this_range_path;

    // ATTN 16
    switch (currentThompsonHeightIdx) {
    case 0:
      {
        this_range_path = dirToMakePath + "aerialHeight0Gradients.yml";
      }
      break;
    case 1:
      {
        this_range_path = dirToMakePath + "aerialHeight1Gradients.yml";
      }
      break;
    case 2:
      {
        this_range_path = dirToMakePath + "aerialHeight2Gradients.yml";
      }
      break;
    case 3:
      {
        this_range_path = dirToMakePath + "aerialHeight3Gradients.yml";
      }
      break;
    default:
      {
        assert(0);
        break;
      }
    }

    mkdir(dirToMakePath.c_str(), 0777);

    //int hbb = pilotTargetBlueBoxNumber;
    //int hbb = 0;

    int topCornerX = reticle.px - (aerialGradientReticleWidth/2);
    int topCornerY = reticle.py - (aerialGradientReticleWidth/2);
    int crows = aerialGradientReticleWidth;
    int ccols = aerialGradientReticleWidth;

    //int crows = bBots[hbb].y - bTops[hbb].y;
    //int ccols = bBots[hbb].x - bTops[hbb].x;
    int maxDim = max(crows, ccols);
    int tRy = (maxDim-crows)/2;
    int tRx = (maxDim-ccols)/2;
    Mat gCrop(maxDim, maxDim, frameGraySobel.type());

    cout << "crows ccols: " << crows << " " << ccols << " ";

    for (int x = 0; x < maxDim; x++) {
      for (int y = 0; y < maxDim; y++) {
        int tx = x - tRx;
        int ty = y - tRy;
        int tCtx = topCornerX + tx;
        int tCty = topCornerY + ty;
        if ( (tx >= 0 && ty >= 0 && ty < crows && tx < ccols) &&
             (tCtx > 0) && (tCty > 0) && (tCtx < imW) && (tCty < imH) ) {
          //gCrop.at<double>(y, x) = frameGraySobel.at<double>(bTops[hbb].y + ty, bTops[hbb].x + tx);
          gCrop.at<double>(y, x) = frameGraySobel.at<double>(tCty, tCtx);
        } else {
          gCrop.at<double>(y, x) = 0.0;
        }
      }
    }
  
    cout << "about to resize" << endl;

    Size toBecome(aerialGradientWidth, aerialGradientWidth);
    cv::resize(gCrop, gCrop, toBecome);


    FileStorage fsvO;
    cout << "capslock + Z: Writing: " << this_range_path << endl;

    fsvO.open(this_range_path, FileStorage::WRITE);

    // ATTN 16
    switch (currentThompsonHeightIdx) {
    case 0:
      {
        fsvO << "aerialHeight0Gradients" << gCrop;
      }
      break;
    case 1:
      {
        fsvO << "aerialHeight1Gradients" << gCrop;
      }
      break;
    case 2:
      {
        fsvO << "aerialHeight2Gradients" << gCrop;
      }
      break;
    case 3:
      {
        fsvO << "aerialHeight3Gradients" << gCrop;
      }
      break;
    default:
      {
        assert(0);
      }
      break;
    }

    lastAerialGradient = gCrop;
    fsvO.release();
  } 
}
END_WORD


WORD(InitializeAndFocusOnNewClass)
CODE(196720)     // capslock + P
virtual void execute() {
  focusedClass = numClasses+newClassCounter;
  char buf[1024];
  sprintf(buf, "autoClass%d_%s", focusedClass, left_or_right_arm.c_str());
  string thisLabelName(buf);
  focusedClassLabel = thisLabelName;
  classLabels.push_back(thisLabelName);
  string dirToMakePath = data_directory + "/" + thisLabelName + "/";
  mkdir(dirToMakePath.c_str(), 0777);
  string rgbDirToMakePath = data_directory + "/" + thisLabelName + "/rgb";
  mkdir(rgbDirToMakePath.c_str(), 0777);
  newClassCounter++;
}
END_WORD
