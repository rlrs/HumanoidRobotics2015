#include "../incVision/vision.h"


using namespace cv;
using namespace std;
using namespace Eigen;



int stateInitColor(Mat imgOrg, Arm arm)
{
    int nextState;

	// set nextState to stay in function
	if(arm.getArmName() == "arm_left")
		nextState = INIT_COLOR_LEFT;
	else if(arm.getArmName() == "arm_right")
		nextState = INIT_COLOR_RIGHT;

    // setting up colors
    if(arm.getArmName() == "arm_left")
    {
        namedWindow("control_left", CV_WINDOW_AUTOSIZE);

        // red
        cvCreateTrackbar("iRHLow",  "control_left", &iRHLow, 179); //Hue (0 - 179)
        cvCreateTrackbar("iRHHigh", "control_left", &iRHHigh, 179);
        cvCreateTrackbar("iRSLow",  "control_left", &iRSLow, 255); //Saturation (0 - 255)
        cvCreateTrackbar("iRSHigh", "control_left", &iRSHigh, 255);
        cvCreateTrackbar("iRVLow",  "control_left", &iRVLow, 255); //Value (0 - 255)
        cvCreateTrackbar("iRVHigh", "control_left", &iRVHigh, 255);

        // yellow
        cvCreateTrackbar("iYHLow",  "control_left", &iYHLow, 179); //Hue (0 - 179)
        cvCreateTrackbar("iYHHigh", "control_left", &iYHHigh, 179);
        cvCreateTrackbar("iYSLow",  "control_left", &iYSLow, 255); //Saturation (0 - 255)
        cvCreateTrackbar("iYSHigh", "control_left", &iYSHigh, 255);
        cvCreateTrackbar("iYVLow",  "control_left", &iYVLow, 255); //Value (0 - 255)
        cvCreateTrackbar("iYVHigh", "control_left", &iYVHigh, 255);

        // blue
        cvCreateTrackbar("iBHLow",  "control_left", &iBHLow, 179); //Hue (0 - 179)
        cvCreateTrackbar("iBHHigh", "control_left", &iBHHigh, 179);
        cvCreateTrackbar("iBSLow",  "control_left", &iBSLow, 255); //Saturation (0 - 255)
        cvCreateTrackbar("iBSHigh", "control_left", &iBSHigh, 255);
        cvCreateTrackbar("iBVLow",  "control_left", &iBVLow, 255); //Value (0 - 255)
        cvCreateTrackbar("iBVHigh", "control_left", &iBVHigh, 255);

    }
    else if(arm.getArmName() == "arm_right")
    {
        namedWindow("control_right", CV_WINDOW_AUTOSIZE);

        // green
        cvCreateTrackbar("iGHLow",  "control_right", &iGHLow, 179); //Hue (0 - 179)
        cvCreateTrackbar("iGHHigh", "control_right", &iGHHigh, 179);
        cvCreateTrackbar("iGSLow",  "control_right", &iGSLow, 255); //Saturation (0 - 255)
        cvCreateTrackbar("iGSHigh", "control_right", &iGSHigh, 255);
        cvCreateTrackbar("iGVLow",  "control_right", &iGVLow, 255); //Value (0 - 255)
        cvCreateTrackbar("iGVHigh", "control_right", &iGVHigh, 255);

        // dark_blue
        cvCreateTrackbar("iDBHLow",  "control_right", &iDBHLow, 179); //Hue (0 - 179)
        cvCreateTrackbar("iDBHHigh", "control_right", &iDBHHigh, 179);
        cvCreateTrackbar("iDBSLow",  "control_right", &iDBSLow, 255); //Saturation (0 - 255)
        cvCreateTrackbar("iDBSHigh", "control_right", &iDBSHigh, 255);
        cvCreateTrackbar("iDBVLow",  "control_right", &iDBVLow, 255); //Value (0 - 255)
        cvCreateTrackbar("iDBVHigh", "control_right", &iDBVHigh, 255);

        // brown
        cvCreateTrackbar("iBRHLow",  "control_right", &iBRHLow, 179); //Hue (0 - 179)
        cvCreateTrackbar("iBRHHigh", "control_right", &iBRHHigh, 179);
        cvCreateTrackbar("iBRSLow",  "control_right", &iBRSLow, 255); //Saturation (0 - 255)
        cvCreateTrackbar("iBRSHigh", "control_right", &iBRSHigh, 255);
        cvCreateTrackbar("iBRVLow",  "control_right", &iBRVLow, 255); //Value (0 - 255)
        cvCreateTrackbar("iBRVHigh", "control_right", &iBRVHigh, 255);
    }

    Mat imgHSV;

    cvtColor(imgOrg, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    // update color values
    if(arm.getArmName() == "arm_left")
    {
        sRedLow         = Scalar(iRHLow, iRSLow, iRVLow);
        sRedHigh        = Scalar(iRHHigh, iRSHigh, iRVHigh);
        sYellowLow      = Scalar(iYHLow, iYSLow, iYVLow);
        sYellowHigh     = Scalar(iYHHigh, iYSHigh, iYVHigh);
        sBlueLow        = Scalar(iBHLow, iBSLow, iBVLow);
        sBlueHigh       = Scalar(iBHHigh, iBSHigh, iBVHigh);
    }
    else if(arm.getArmName() == "arm_right")
    {
        sGreenLow       = Scalar(iGHLow, iGSLow, iGVLow);
        sGreenHigh      = Scalar(iGHHigh, iGSHigh, iGVHigh);
        sDarkBlueLow    = Scalar(iDBHLow, iDBSLow, iDBVLow);
        sDarkBlueHigh   = Scalar(iDBHHigh, iDBSHigh, iDBVHigh);
        sBrownLow       = Scalar(iBRHLow, iBRSLow, iBRVLow);
        sBrownHigh      = Scalar(iBRHHigh, iBRSHigh, iBRVHigh);
    }


    Mat imgTmp1, imgTmp2;
    Mat imgThreshold;

    // get HSV-values
    if(arm.getArmName() == "arm_left")
    {
        inRange(imgHSV, sRedLow, sRedHigh, imgTmp1);
        inRange(imgHSV, sYellowLow, sYellowHigh, imgTmp2);
        bitwise_or(imgTmp1, imgTmp2, imgThreshold);
        imgTmp1 = imgThreshold;
        inRange(imgHSV, sBlueLow, sBlueHigh, imgTmp2);
        bitwise_or(imgTmp1, imgTmp2, imgThreshold);
    }
    else if(arm.getArmName() == "arm_right")
    {
        inRange(imgHSV, sGreenLow, sGreenHigh, imgTmp1);
        inRange(imgHSV, sDarkBlueLow, sDarkBlueHigh, imgTmp2);
        bitwise_or(imgTmp1, imgTmp2, imgThreshold);
        imgTmp1 = imgThreshold;
        inRange(imgHSV, sBrownLow, sBrownHigh, imgTmp2);
        bitwise_or(imgTmp1, imgTmp2, imgThreshold);
    }

    //morphological opening (remove small objects from the foreground)
    erode(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    //morphological closing (fill small holes in the foreground)
    dilate(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    // remove background
    Mat withoutBG = Mat::zeros(imgOrg.size(), imgOrg.type());
    for(int y = 0; y < imgOrg.rows; y++)
    {
        for(int x = 0; x < imgOrg.cols; x++)
        {
            if(imgThreshold.at<uchar>(y, x) == MAX_THRESH_VAL)
                withoutBG.at<Vec3b>(Point(x, y)) = imgOrg.at<Vec3b>(Point(x, y));
        }
    }
    imshow("withoutBG", withoutBG);

    //wait for 'enter' key press for 30ms. If 'enter' key is pressed, break loop
    if (waitKey(30) == PRESS_ENTER)
    {
		// set next state
        if(arm.getArmName() == "arm_left")
		{
			nextState = DESTROY_WINDOW_INIT_COLOR_LEFT;
            cout << "seting up colors for left arm finished" << endl;
		}
        else if(arm.getArmName() == "arm_right")
		{
			nextState = DESTROY_WINDOW_INIT_COLOR_RIGHT;
            cout << "seting up colors for right arm finished" << endl;
		}
    }

    return nextState;
}



int stateInitArm(Mat imgOrg, Arm *arm)
{
    int nextState;

    if(arm->getArmName() == "arm_left")
        nextState = INIT_ARM_RIGHT;
    else if(arm->getArmName() == "arm_right")
        nextState = RUN;

    // convert into HSV
    Mat imgHSV;
    cvtColor(imgOrg, imgHSV, COLOR_BGR2HSV);

    Mat withoutBG = Mat::zeros(imgOrg.size(), imgOrg.type());

    // set centers of points
    arm->setJ1Center(getJCenter(imgHSV.clone(), imgOrg, withoutBG, arm->getJ1Color()));
    arm->setJ2Center(getJCenter(imgHSV.clone(), imgOrg, withoutBG, arm->getJ2Color()));
    arm->setJ3Center(getJCenter(imgHSV.clone(), imgOrg, withoutBG, arm->getJ3Color()));

    // get center point of each joint
    Point p1 = arm->getJ1Center();
    Point p2 = arm->getJ2Center();
    Point p3 = arm->getJ3Center();

    // distance between p1 and p2
    int dx1 = p1.x - p2.x;
    int dy1 = p1.y - p2.y;

    // distance between p2 and p3
    int dx2 = p2.x - p3.x;
    int dy2 = p2.y - p3.y;

    // set bone length
    arm->setBone1(sqrt(dx1 * dx1 + dy1 * dy1));
    arm->setBone2(sqrt(dx2 * dx2 + dy2 * dy2));

    if(arm->getArmName() == "arm_left")
        cout << "initialization of left arm finished\n";
    else if(arm->getArmName() == "arm_right")
        cout << "initialization of right arm finished\n";

    return nextState;
}



void drawModel(Mat imgOrg, Arm *arm)
{
    // all 3 joints
    if(arm->getJ1Center().x != 0 && arm->getJ1Center().y != 0 &&
       arm->getJ2Center().x != 0 && arm->getJ2Center().y != 0 &&
       arm->getJ3Center().x != 0 && arm->getJ3Center().y != 0)
    {
        // draw joints
        circle(imgOrg, arm->getJ1Center(), jointRadius, jointColor);
        circle(imgOrg, arm->getJ2Center(), jointRadius, jointColor);
        circle(imgOrg, arm->getJ3Center(), jointRadius, jointColor);

        // draw lines between joints
        line(imgOrg, arm->getJ1Center(), arm->getJ2Center(), lineColor);
        line(imgOrg, arm->getJ2Center(), arm->getJ3Center(), lineColor);

        imshow("orig & model", imgOrg);
    }

    // not enought joints jound
    else
    {
        // print info
        cout << "not enough joints found in " << arm->getArmName() << endl;
        cout << "j1 " << arm->getJ1Center() << endl;
        cout << "j2 " << arm->getJ2Center() << endl;
        cout << "j3 " << arm->getJ3Center() << endl;
    }
}



int calculatePosition(Arm *arm)
{
    Point p1 = arm->getJ1Center();
    Point p2 = arm->getJ2Center();
    Point p3 = arm->getJ3Center();

    // check if all joints are detected
    if((p1.x != 0 || p1.y != 0) && (p2.x != 0 || p2.y != 0) && (p3.x != 0 || p2.y != 0))
    {
        int link1 = arm->getBone1();
        int link2 = arm->getBone2();

//        cout << arm->getArmName() << endl;
//        cout << p1 << " " << p2 << " " << p3 << endl;


        // LINK 1
        // calc relative joint position from shoulder to elbow
        int dPicX2 = p2.x - p1.x;
        int dPicY2 = p2.y - p1.y;

        // calc distance from shoulder to elbow
        double dPic1 = sqrt(dPicX2 * dPicX2 + dPicY2 * dPicY2);


        // calc angle between shoulder and elbow
        double alpha1 = atan((double)dPicX2 / dPicY2);
        // calc depth angle
        double alpha2 = acos((double)link1 / dPic1);

        // translate link1 from human to NAO
        double link1NAO = dPic1 / link1 * BONE1ROBOT;


        // calc NAO coord
        Vector3d coordNAO1;
//        coordNAO1(0) = link1NAO * sin(alpha1);
//        coordNAO1(1) = link1NAO * sin(alpha2);
//        coordNAO1(2) = - link1NAO * cos(alpha1);
        coordNAO1(0) = link1NAO * sin(alpha2);
        coordNAO1(1) = link1NAO * sin(alpha1);
        coordNAO1(2) = - link1NAO * cos(alpha1);

        //cout << "coord1 " << coordNAO1(0) << " " << coordNAO1(1) << " " << coordNAO1(2) << endl;



        // LINK 2
        // calc relative joint position from elbow to wrist
        int dPicX3 = p3.x - p2.x;
        int dPicY3 = p3.y - p2.y;

        // calc distance from elbow to wrist
        double dPic2 = sqrt(dPicX3 * dPicX3 + dPicY3 * dPicY3);


        // calc angle between elbow and wrist
        alpha1 = atan((double)dPicX3 / dPicY3);
        // calc depth angle
        alpha2 = acos((double)link2 / dPic2);

        // translate link2 from human to NAO
        double link2NAO = dPic2 / link2 * BONE2ROBOT;


        // calc NAO coord
        Vector3d coordNAO2;
//        coordNAO2(0) = link2NAO * sin(alpha1) + coordNAO1(0);
//        coordNAO2(1) = link2NAO * sin(alpha2) + coordNAO1(1);
//        coordNAO2(2) = - link2NAO * cos(alpha1) + coordNAO1(2);
        coordNAO2(0) = link2NAO * sin(alpha2) + coordNAO1(0);
        coordNAO2(1) = link2NAO * sin(alpha1) + coordNAO1(1);
        coordNAO2(2) = - link2NAO * cos(alpha1) + coordNAO1(2);

        //cout << "coord2 " << coordNAO2(0) << " " << coordNAO2(1) << " " << coordNAO2(2) << endl;

        arm->setJ2Coord(coordNAO1);
        arm->setJ3Coord(coordNAO2);
    }
    else
        return FAILURE;

    return SUCCESS;
}



int runArmTracking(Mat imgOrg, Arm *arm_left, Arm *arm_right)
{
	if(SHOW_ORIGINAL_IMG == ON)
        imshow("imgOrg", imgOrg);

    // convert into HSV
    Mat imgHSV;
    cvtColor(imgOrg, imgHSV, COLOR_BGR2HSV);

    Mat withoutBG = Mat::zeros(imgOrg.size(), imgOrg.type());

    arm_left->setJ1Center(getJCenter(imgHSV.clone(), imgOrg, withoutBG, arm_left->getJ1Color()));
    arm_left->setJ2Center(getJCenter(imgHSV.clone(), imgOrg, withoutBG, arm_left->getJ2Color()));
    arm_left->setJ3Center(getJCenter(imgHSV.clone(), imgOrg, withoutBG, arm_left->getJ3Color()));

    arm_right->setJ1Center(getJCenter(imgHSV.clone(), imgOrg, withoutBG, arm_right->getJ1Color()));
    arm_right->setJ2Center(getJCenter(imgHSV.clone(), imgOrg, withoutBG, arm_right->getJ2Color()));
    arm_right->setJ3Center(getJCenter(imgHSV.clone(), imgOrg, withoutBG, arm_right->getJ3Color()));

    if(SHOW_WITHOUT_BG_IMG == ON)
        imshow("only points", withoutBG);

    // draw model into image
    drawModel(imgOrg, arm_left);
    drawModel(imgOrg, arm_right);

    // calculate position
	if(calculatePosition(arm_left) == SUCCESS && calculatePosition(arm_right) == SUCCESS)
		return SUCCESS;
	else
		return FAILURE;

/*    if(calculatePosition(&arm_left) == SUCCESS)
    {
        cout << arm_left->getArmName() << endl;

        Vector3d jointLeft;

        jointLeft = arm_left->getJ1Coord();
        cout << "j1   " << jointLeft(0) << "  " << jointLeft(1) << "  " << jointLeft(2) << endl;

        jointLeft = arm_left->getJ2Coord();
        cout << "j2   " << jointLeft(0) << "  " << jointLeft(1) << "  " << jointLeft(2) << endl;

        jointLeft = arm_left.getJ3Coord();
        cout << "j3   " << jointLeft(0) << "  " << jointLeft(1) << "  " << jointLeft(2) << endl;
    }
    else
        cout << "not all joints for left arm found!\n";


    if(calculatePosition(&arm_right) == SUCCESS)
    {
        cout << arm_right->getArmName() << endl;

        Vector3d jointRight;

        jointRight = arm_right->getJ1Coord();
        cout << "j1   " << jointRight(0) << "  " << jointRight(1) << "  " << jointRight(2) << endl;

        jointRight = arm_right->getJ2Coord();
        cout << "j2   " << jointRight(0) << "  " << jointRight(1) << "  " << jointRight(2) << endl;

        jointRight = arm_right->getJ3Coord();
        cout << "j3   " << jointRight(0) << "  " << jointRight(1) << "  " << jointRight(2) << endl << endl << endl;
    }
    else
        cout << "not all joints for right arm found\n";
*/

}

