// -------------------------------------------------------------------
// PltApp3D.C 
// -------------------------------------------------------------------
#include "PltApp.H"
#if defined(BL_PARALLELVOLUMERENDER)
#include <PVolRender.H>
#endif

#include "Quaternion.H"

// -------------------------------------------------------------------
void PltApp::DoExposeTransDA() {
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
  Widget currentAutoDraw = (transDetached? wDAutoDraw : wAutoDraw);
  if(XmToggleButtonGetState(currentAutoDraw)) {
      projPicturePtr->DrawPicture();
  } else {
      projPicturePtr->DrawBoxes(minDrawnLevel, maxDrawnLevel);
  }
  if(labelAxes) {
    if(XmToggleButtonGetState(currentAutoDraw)) {
      projPicturePtr->MakeBoxes();
    }
    projPicturePtr->LabelAxes();
  }
#else
      projPicturePtr->DrawBoxes(minDrawnLevel, maxDrawnLevel);
  if(labelAxes) {
    projPicturePtr->LabelAxes();
  }
#endif
}


// -------------------------------------------------------------------
void PltApp::DoTransInput(Widget w, XtPointer, XtPointer call_data) {
    Real temp;
  AmrQuaternion quatRotation, quatRotation2;
  AmrQuaternion newRotation, newRotation2;
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
    Widget currentAutoDraw = (transDetached? wDAutoDraw : wAutoDraw);
#endif
  XmDrawingAreaCallbackStruct* cbs =
        (XmDrawingAreaCallbackStruct*) call_data;
  if(cbs->event->xany.type == ButtonPress) {
    servingButton = cbs->event->xbutton.button;
    startX = endX = cbs->event->xbutton.x;
    startY = endY = cbs->event->xbutton.y;
    acc = 0;
  }
  if(servingButton==1) {
      endX = cbs->event->xbutton.x;
      endY = cbs->event->xbutton.y;
     if (cbs->event->xbutton.state & ShiftMask)
      {
          //Real x = 0; Real y = 0;
          //viewTrans.GetTranslation(x, y);
          viewTrans.MakeTranslation(startX, startY, endX, endY, 
                                    projPicturePtr->longestBoxSide/64.);
      } else {
          quatRotation = viewTrans.GetRotation();
          quatRotation2 = viewTrans.GetRenderRotation();
          newRotation = viewTrans.Screen2Quat(projPicturePtr->ImageSizeH()-startX,
                                              startY,
                                              projPicturePtr->ImageSizeH()-endX,
                                              endY,
                                              projPicturePtr->longestBoxSide/64.);
// this last number scales the rotations correctly
          newRotation2 = viewTrans.Screen2Quat(projPicturePtr->ImageSizeH()-startX,
                                               projPicturePtr->ImageSizeV()-startY,
                                               projPicturePtr->ImageSizeH()-endX,
                                               projPicturePtr->ImageSizeV()-endY,
                                               projPicturePtr->longestBoxSide/64.);
          quatRotation = newRotation * quatRotation;
          viewTrans.SetRotation(quatRotation);
          quatRotation2 = newRotation2 * quatRotation2;
          viewTrans.SetRenderRotation(quatRotation2);
      }
      viewTrans.MakeTransform();
      
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
      if(XmToggleButtonGetState(currentAutoDraw)) {
          DoRender(w, NULL, NULL);
      } else {
          if (showing3dRender) showing3dRender = false;
          projPicturePtr->MakeBoxes();
          XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
      }
#else
      projPicturePtr->MakeBoxes();
      XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
#endif
      
      DoExposeTransDA();
      acc += 1;
      if(acc==10) {
          startX = cbs->event->xbutton.x;
          startY = cbs->event->xbutton.y;
          acc = 0;
      }
  } 

  if(servingButton==2) {
      endX = cbs->event->xbutton.x;
      endY = cbs->event->xbutton.y;
      Real change = (startY-endY)*0.003;
      quatRotation = viewTrans.GetRotation();
      quatRotation2 = viewTrans.GetRenderRotation();
// should replace the trigonometric equations...
      newRotation = AmrQuaternion(cos(-change), 0, 0, sin(-change));
      newRotation2 = AmrQuaternion(cos(change), 0, 0, sin(change));

      quatRotation = newRotation * quatRotation;
      viewTrans.SetRotation(quatRotation);
      quatRotation2 = newRotation2 * quatRotation2;
      viewTrans.SetRenderRotation(quatRotation2);
      viewTrans.MakeTransform();
      
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
      if(XmToggleButtonGetState(currentAutoDraw)) {
          DoRender(w, NULL, NULL);
      } else {
          if (showing3dRender) showing3dRender = false;
          projPicturePtr->MakeBoxes();
          XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
      }
#else
      projPicturePtr->MakeBoxes();
      XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
#endif
      DoExposeTransDA();
      acc += 1;
      if(acc==10) {
          startX = cbs->event->xbutton.x;
          startY = cbs->event->xbutton.y;
          acc = 0;
      }
  } 
  
  if(servingButton==3) {
      endX = cbs->event->xbutton.x;
      endY = cbs->event->xbutton.y;
      Real ss = viewTrans.GetScale();
      temp = ss+(endY-startY)*0.003;
      if(temp<0.1) {
          temp = 0.1;
      }
      viewTrans.SetScale(temp);
      viewTrans.MakeTransform();
      
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
      if(XmToggleButtonGetState(currentAutoDraw)) {
          DoRender(w, NULL, NULL);
      } else {
          if (showing3dRender) showing3dRender = false;
          projPicturePtr->MakeBoxes();
          XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
      }
#else
      projPicturePtr->MakeBoxes();
      XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
#endif
      DoExposeTransDA();
      acc += 1;
      if(acc==15) {
          startX = cbs->event->xbutton.x;
          startY = cbs->event->xbutton.y;
      acc = 0;
      }
  } 
  if(cbs->event->xany.type == ButtonRelease) {
      startX = endX = 0;
      startY = endY = 0;
      acc = 0;
  }
}  // end DoTransInput(...)


// -------------------------------------------------------------------
void PltApp::CBTransResize(Widget w, XtPointer client_data, XtPointer call_data) {
    PltApp *obj = (PltApp *) client_data;
    obj->DoTransResize(w, client_data, call_data);
}


// -------------------------------------------------------------------
void PltApp::DoAttach(Widget, XtPointer, XtPointer) {
  transDetached = false;

  //Query wDLight for its state:
  Widget getHistory;
  XtVaGetValues(wDLight, XmNmenuHistory, &getHistory, NULL);
  int renderMode = ( getHistory == wDLightItems[0] ? 0 : 1 );

  //Query wDClassify for its state:
  XtVaGetValues(wDClassify, XmNmenuHistory, &getHistory, NULL);
  int classMode = ( getHistory == wDClassifyItems[0] ? 0 : 1 );

  XtRemoveCallback(wTransDA, XmNinputCallback, &PltApp::CBTransInput, NULL);
  XtRemoveCallback(wTransDA, XmNresizeCallback, &PltApp::CBTransResize, NULL);
  XtRemoveEventHandler(wTransDA, ExposureMask, false,
	(XtEventHandler) CBExposeTransDA, (XtPointer) this);

  XtDestroyWidget(wDetachTopLevel);

  XtManageChild(wOrient);
  XtManageChild(wLabelAxes);
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
  XtManageChild(wRender);
  XtManageChild(wAutoDraw);
  XtManageChild(wReadTransfer);

  XtVaSetValues(wLight, XmNmenuHistory, wLightItems[renderMode], NULL);
  XtVaSetValues(wClassify, XmNmenuHistory, wClassifyItems[classMode], NULL);

  XtManageChild(wLight);
  XtManageChild(wClassify);
#endif
  XtManageChild(wDetach);

  wTransDA = XtVaCreateManagedWidget("transDA", xmDrawingAreaWidgetClass,
		wPlotArea,
		XmNtranslations,	XtParseTranslationTable(trans),
		XmNleftAttachment,	XmATTACH_WIDGET,
		//XmNleftWidget,		wPlotPlane[XPLANE],
		XmNleftWidget,		wScrollArea[XPLANE],
		XmNleftOffset,		WOFFSET,
		XmNtopAttachment,	XmATTACH_WIDGET,
		XmNtopWidget,		wOrient,
		XmNtopOffset,		WOFFSET,
		XmNrightAttachment,	XmATTACH_FORM,
		XmNrightOffset,		WOFFSET,
		XmNbottomAttachment,	XmATTACH_FORM,
		XmNbottomOffset,	WOFFSET,
		NULL);
  XtAddCallback(wTransDA, XmNinputCallback, &PltApp::CBTransInput,
		(XtPointer) this);
  XtAddCallback(wTransDA, XmNresizeCallback, &PltApp::CBTransResize,
		(XtPointer) this);
  XtAddEventHandler(wTransDA, ExposureMask, false,
  	(XtEventHandler) CBExposeTransDA,
	(XtPointer) this);
  projPicturePtr->SetDrawingArea(wTransDA);
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
    XmString sAutoDraw = XmStringCreateSimple("Autodraw");
    XtVaSetValues(wAutoDraw, XmNlabelString, sAutoDraw, NULL);
    XmStringFree(sAutoDraw);
    XmToggleButtonSetState(wAutoDraw, 
                           XmToggleButtonGetState(wDAutoDraw),
                           false);
#endif
  DoTransResize(wTransDA, NULL, NULL);
}


// -------------------------------------------------------------------
void PltApp::DoDetach(Widget, XtPointer, XtPointer) {
  int i;
  Position xpos, ypos;
  Dimension wdth, hght;

  //Query wLight here for its state:
  Widget getHistory;
  XtVaGetValues(wLight, XmNmenuHistory, &getHistory, NULL);
  int renderMode = ( getHistory == wLightItems[0] ? 0 : 1 );

  //Query wClassify here for its state:
  XtVaGetValues(wClassify, XmNmenuHistory, &getHistory, NULL);
  int classMode = ( getHistory == wClassifyItems[0] ? 0 : 1 );
  
  transDetached = true;
  XtUnmanageChild(wTransDA);
  XtUnmanageChild(wOrient);
  XtUnmanageChild(wLabelAxes);
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
  XtUnmanageChild(wRender);
  XtUnmanageChild(wAutoDraw);
  XtUnmanageChild(wReadTransfer);
  XtUnmanageChild(wLight);
  XtUnmanageChild(wClassify);
#endif
  XtUnmanageChild(wDetach);
  XtRemoveCallback(wTransDA, XmNinputCallback, &PltApp::CBTransInput, NULL);
  XtRemoveCallback(wTransDA, XmNresizeCallback, &PltApp::CBTransResize, NULL);
  XtRemoveEventHandler(wTransDA, ExposureMask, false,
	(XtEventHandler) CBExposeTransDA, (XtPointer) this);
  XtDestroyWidget(wTransDA);

  XtVaGetValues(wAmrVisTopLevel, XmNx, &xpos, XmNy, &ypos,
	XmNwidth, &wdth, XmNheight, &hght, NULL);

  aString outbuf = "XYZ ";
  outbuf += fileName;
  strcpy(buffer, outbuf.c_str());

  wDetachTopLevel = XtVaCreatePopupShell(buffer,
	topLevelShellWidgetClass, wAmrVisTopLevel,
	XmNwidth,		500,
	XmNheight,		500,
	XmNx,			xpos+wdth/2,
	XmNy,			ypos+hght/2,
	NULL);

  if(GAptr->PVisual() 
     != XDefaultVisual(GAptr->PDisplay(), GAptr->PScreenNumber())) {
      XtVaSetValues(wDetachTopLevel, XmNvisual, GAptr->PVisual(),
                    XmNdepth, 8, NULL);
  }

  wDetachForm = XtVaCreateManagedWidget("detachform",
	xmFormWidgetClass, wDetachTopLevel,
	NULL);

  i=0;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM);   i++;
  XtSetArg(args[i], XmNtopOffset, WOFFSET);    i++;
  XtSetArg(args[i], XmNrightAttachment, XmATTACH_FORM);      i++;
  XtSetArg(args[i], XmNrightOffset, WOFFSET);      i++;
  XmString sAttach = XmStringCreateSimple("Attach");
  XtSetArg(args[i], XmNlabelString, sAttach);    i++;
  wAttach = XmCreatePushButton(wDetachForm, "attach", args, i);
  XmStringFree(sAttach);
  AddStaticCallback(wAttach, XmNactivateCallback, &PltApp::DoAttach);
  XtManageChild(wAttach);

  i=0;
  XmString sOrient = XmStringCreateSimple("0");
  XtSetArg(args[i], XmNlabelString, sOrient); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNleftOffset, WOFFSET); i++;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNtopOffset, WOFFSET); i++;
  wDOrient = XmCreatePushButton(wDetachForm, "dorient", args, i);
  XmStringFree(sOrient);
  AddStaticCallback(wDOrient, XmNactivateCallback, &PltApp::DoOrient);
  XtManageChild(wDOrient);

  i=0;
  XmString sLabel = XmStringCreateSimple("XYZ");
  XtSetArg(args[i], XmNlabelString, sLabel); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNleftWidget, wDOrient); i++;
  XtSetArg(args[i], XmNleftOffset, WOFFSET); i++;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNtopOffset, WOFFSET); i++;
  wDLabelAxes = XmCreatePushButton(wDetachForm, "dlabel", args, i);
  XmStringFree(sLabel);
  AddStaticCallback(wDLabelAxes, XmNactivateCallback, &PltApp::DoLabelAxes);
  XtManageChild(wDLabelAxes);

#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
  i=0;
  XmString sRender = XmStringCreateSimple("Draw");
  XtSetArg(args[i], XmNlabelString, sRender); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNleftWidget, wDLabelAxes); i++;
  XtSetArg(args[i], XmNleftOffset, WOFFSET); i++;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNtopOffset, WOFFSET); i++;
  wDRender = XmCreatePushButton(wDetachForm, "drender", args, i);
  XmStringFree(sRender);
  AddStaticCallback(wDRender, XmNactivateCallback, &PltApp::DoRender);
  XtManageChild(wDRender);

  i=0;
  XmString sAutoDraw;
  sAutoDraw = XmStringCreateSimple("AutoDraw");
  XtSetArg(args[i], XmNlabelString, sAutoDraw); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNleftWidget, wDRender); i++;
  XtSetArg(args[i], XmNleftOffset, WOFFSET); i++;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNtopOffset, WOFFSET); i++;
  wDAutoDraw = XmCreateToggleButton(wDetachForm, "dautodraw", args, i);
  XmStringFree(sAutoDraw);
  AddStaticCallback(wDAutoDraw, XmNvalueChangedCallback, &PltApp::DoAutoDraw);
  XmToggleButtonSetState(wDAutoDraw, XmToggleButtonGetState(wAutoDraw),
                         false);
  XtManageChild(wDAutoDraw);

  i=0;
  XmString sReadTrans = XmStringCreateSimple("Trans");
  XtSetArg(args[i], XmNlabelString, sReadTrans); i++;
  XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
  XtSetArg(args[i], XmNleftWidget, wDAutoDraw); i++;
  XtSetArg(args[i], XmNleftOffset, WOFFSET); i++;
  XtSetArg(args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  XtSetArg(args[i], XmNtopOffset, WOFFSET); i++;
  wDReadTransfer = XmCreatePushButton(wDetachForm, "dreadtrans", args, i);
  XmStringFree(sReadTrans);
  AddStaticCallback(wDReadTransfer, XmNactivateCallback,
                    &PltApp::DoReadTransferFile);
  XtManageChild(wDReadTransfer);

// Render Mode Menu
  i=0;
  wDLightOptions = XmCreatePulldownMenu(wDetachForm,"lightingoptions", args, i);
  XtVaSetValues(wDLightOptions, XmNuserData, this, NULL);
  XmString DLightItems[2];
  DLightItems[0] = XmStringCreateSimple("Light");
  DLightItems[1] = XmStringCreateSimple("Value");
  

  for(int j = 0; j<2; j++)
  {
    XtSetArg(args[0], XmNlabelString, DLightItems[j]);
    wDLightItems[j] = XmCreatePushButtonGadget(wDLightOptions,
                                              "light", args, 1);
    XtAddCallback(wDLightItems[j], XmNactivateCallback,
		  &PltApp::CBRenderModeMenu, (XtPointer)j);
  }
  XmStringFree(DLightItems[0]); XmStringFree(DLightItems[1]);
  XtManageChildren(wDLightItems, 2);
  
  

    i=0;
    XtSetArg(args[i], XmNsubMenuId, wDLightOptions); i++;
    XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
    XtSetArg(args[i], XmNleftWidget, wDReadTransfer); i++;
    XtSetArg(args[i], XmNleftOffset, WOFFSET); i++;
    XtSetArg(args[i], XmNtopAttachment, XmATTACH_POSITION); i++;
    XtSetArg(args[i], XmNtopPosition, 0); i++;
    XtSetArg(args[i], XmNmenuHistory, wDLightItems[renderMode]);  i++;
    wDLight = XmCreateOptionMenu(wDetachForm, "lighting", args, i);

    //set this string to "" for cray
  XmString newDLtring = XmStringCreateSimple("");
  XtVaSetValues(XmOptionLabelGadget(wDLight),
                XmNlabelString, newDLtring,
                NULL);
  XmStringFree(newDLtring);
  
  XtManageChild(wDLight);



// Classification Mode Menu

  i=0;
  wDClassifyOptions = XmCreatePulldownMenu(wDetachForm,"classoptions", args, i);
  XtVaSetValues(wDClassifyOptions, XmNuserData, this, NULL);
  XmString DClassifyItems[2];
  DClassifyItems[0] = XmStringCreateSimple("OT");
  DClassifyItems[1] = XmStringCreateSimple("PC");
  

  for(int j = 0; j<2; j++)
  {
    XtSetArg(args[0], XmNlabelString, DClassifyItems[j]);
    wDClassifyItems[j] = XmCreatePushButtonGadget(wDClassifyOptions,
                                              "class", args, 1);
    XtAddCallback(wDClassifyItems[j], XmNactivateCallback,
		  &PltApp::CBClassifyMenu, (XtPointer)j);
  }
  XmStringFree(DClassifyItems[0]); XmStringFree(DClassifyItems[1]);
  XtManageChildren(wDClassifyItems, 2);
  
  

    i=0;
    XtSetArg(args[i], XmNsubMenuId, wDClassifyOptions); i++;
    XtSetArg(args[i], XmNleftAttachment, XmATTACH_WIDGET); i++;
    XtSetArg(args[i], XmNleftWidget, wDLight); i++;
    XtSetArg(args[i], XmNleftOffset, WOFFSET); i++;
    XtSetArg(args[i], XmNtopAttachment, XmATTACH_POSITION); i++;
    XtSetArg(args[i], XmNtopPosition, 0); i++;
    XtSetArg(args[i], XmNmenuHistory, wDClassifyItems[classMode]);  i++;
    wDClassify = XmCreateOptionMenu(wDetachForm, "lighting", args, i);

    //set this string to "" for cray
  XmString newDCtring = XmStringCreateSimple("");
  XtVaSetValues(XmOptionLabelGadget(wDClassify),
                XmNlabelString, newDCtring,
                NULL);
  XmStringFree(newDCtring);
  
  XtManageChild(wDClassify);
 
#endif

  wTransDA = XtVaCreateManagedWidget("detachDA", xmDrawingAreaWidgetClass,
		wDetachForm,
		XmNtranslations,	XtParseTranslationTable(trans),
                XmNleftAttachment,      XmATTACH_FORM,
		XmNleftOffset,		WOFFSET,
		XmNtopAttachment,	XmATTACH_WIDGET,
		XmNtopWidget,		wDOrient,
		XmNtopOffset,		WOFFSET,
		XmNrightAttachment,	XmATTACH_FORM,
		XmNrightOffset,		WOFFSET,
		XmNbottomAttachment,	XmATTACH_FORM,
		XmNbottomOffset,	WOFFSET,
		NULL);
  XtAddCallback(wTransDA, XmNinputCallback, &PltApp::CBTransInput,
		(XtPointer) this);
  projPicturePtr->SetDrawingArea(wTransDA);

  XtPopup(wDetachTopLevel, XtGrabNone);
  XSetWindowColormap(GAptr->PDisplay(), XtWindow(wDetachTopLevel),
        pltPaletteptr->GetColormap());
  XSetWindowColormap(GAptr->PDisplay(), XtWindow(wTransDA),
        pltPaletteptr->GetColormap());

  XtAddCallback(wTransDA, XmNresizeCallback, &PltApp::CBTransResize,
		(XtPointer) this);
  XtAddEventHandler(wTransDA, ExposureMask, false,
  	(XtEventHandler) CBExposeTransDA, (XtPointer) this);

  DoTransResize(wTransDA, NULL, NULL);
}


// -------------------------------------------------------------------
void PltApp::DoTransResize(Widget w, XtPointer, XtPointer) {
  Dimension wdth, hght;

  XSetWindowColormap(GAptr->PDisplay(), XtWindow(wTransDA),
                     pltPaletteptr->GetColormap());

  XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
  XtVaGetValues(wTransDA, XmNwidth, &wdth, XmNheight, &hght, NULL);
  daWidth = wdth;
  daHeight = hght;
  projPicturePtr->SetDrawingAreaDimensions(daWidth, daHeight);
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
  Widget currentAutoDraw = (transDetached? wDAutoDraw : wAutoDraw);
  if(XmToggleButtonGetState(currentAutoDraw)) {
    DoRender(w, NULL, NULL);
  } else {
        if (showing3dRender) showing3dRender = false;
    projPicturePtr->MakeBoxes();
  }
#else
    projPicturePtr->MakeBoxes();
#endif
  DoExposeTransDA();
}


// -------------------------------------------------------------------
void PltApp::DoReadTransferFile(Widget, XtPointer, XtPointer) {
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
#if defined(BL_VOLUMERENDER)
  VolRender *volRender = projPicturePtr->GetVolRenderPtr();
#endif
#if defined(BL_PARALLELVOLUMERENDER)
  PVolRender *volRender = projPicturePtr->GetVolRenderPtr();
#endif
  projPicturePtr->ReadTransferFile("vpramps.dat");

  if( ! volRender->SWFDataAllocated()) {
    return;
  }
  if( ! volRender->SWFDataValid()) {
    return;
  }
  if(volRender->VPDataValid()) {
      cout<<"\thave valid VPData"<<endl;
    volRender->MakeVPData();    // reclassify the data
  }
  projPicturePtr->MakePicture();

  DoExposeTransDA();
#endif
}


// -------------------------------------------------------------------
void PltApp::Clear() {
  XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
  Widget currentAutoDraw = (transDetached? wDAutoDraw : wAutoDraw);
  XmToggleButtonSetState(currentAutoDraw, false, false);
#endif
  viewTrans.SetRotation(AmrQuaternion(1, 0, 0, 0));
  viewTrans.MakeTransform();
  DoExposeTransDA();
}


// -------------------------------------------------------------------
void PltApp::DoAutoDraw(Widget w, XtPointer, XtPointer) {
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
    Widget currentAutoDraw = (transDetached? wDAutoDraw : wAutoDraw);
    if(XmToggleButtonGetState(currentAutoDraw)) {
        DoRender(w, NULL, NULL);
    } else {
        if (showing3dRender) showing3dRender = false;
        projPicturePtr->MakeBoxes();
    }
    // XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
    // DoExposeTransDA();
#endif
}

// -------------------------------------------------------------------

void PltApp::DoOrient(Widget w, XtPointer, XtPointer) {
    viewTrans.SetRotation(AmrQuaternion());
    viewTrans.SetRenderRotation(AmrQuaternion());
    viewTrans.ResetTranslation();
  viewTrans.MakeTransform();
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
  Widget currentAutoDraw = (transDetached? wDAutoDraw : wAutoDraw);
  if(XmToggleButtonGetState(currentAutoDraw)) {
    DoRender(w, NULL, NULL);
  } else {
        if (showing3dRender) showing3dRender = false;
        projPicturePtr->MakeBoxes();
  }
#else
    projPicturePtr->MakeBoxes();
#endif
  XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
  DoExposeTransDA();
}


// -------------------------------------------------------------------
void PltApp::SetLightingModel() {
#if defined(BL_VOLUMERENDER)
    lightingModel = true;
    projPicturePtr->GetVolRenderPtr()->SetLightingModel(true);
#endif
}

void PltApp::SetValueModel() {
#if defined(BL_VOLUMERENDER)
    lightingModel = false;
    projPicturePtr->GetVolRenderPtr()->SetLightingModel(false);
#endif
}

void PltApp::SetOctreeAlgorithm() {
    preClassify = false;
    projPicturePtr->GetVolRenderPtr()->SetPreClassifyAlgorithm(false);
}

void PltApp::SetPreClassifyAlgorithm() {
    preClassify = true;
    projPicturePtr->GetVolRenderPtr()->SetPreClassifyAlgorithm(true);
}

void PltApp::CBTransInput(Widget w, XtPointer client_data, XtPointer call_data) {
  PltApp *obj = (PltApp *) client_data;
  obj->DoTransInput(w, client_data, (XmDrawingAreaCallbackStruct *) call_data);
}

void PltApp::CBRenderModeMenu(Widget w, XtPointer item_no, XtPointer client_data) {
    unsigned long getobj;
    XtVaGetValues(XtParent(w), XmNuserData, &getobj, NULL);
    PltApp *obj = (PltApp *) getobj;
    obj->DoRenderModeMenu(w, item_no, client_data);
}

void PltApp::DoRenderModeMenu(Widget w, XtPointer item_no, XtPointer client_data) {
    if(item_no == (XtPointer)0) { //Use Lighting model
        if (lightingModel)
            return;
        SetLightingModel();
    } else if (item_no == (XtPointer)1) { //Use Value model
        if ( ! lightingModel)
            return;
        SetValueModel();
    }
    projPicturePtr->GetVolRenderPtr()->InvalidateVPData();
    if (XmToggleButtonGetState(wAutoDraw)||showing3dRender) {
        DoRender(w, NULL, NULL);
    }
}


void PltApp::CBClassifyMenu(Widget w, XtPointer item_no, XtPointer client_data) 
{
    unsigned long getobj;
    XtVaGetValues(XtParent(w), XmNuserData, &getobj, NULL);
    PltApp *obj = (PltApp *) getobj;
    obj->DoClassifyMenu(w, item_no, client_data);
}

void PltApp::DoClassifyMenu(Widget w, XtPointer item_no, XtPointer client_data) 
{
    if(item_no == (XtPointer)0) { //Use Octree Mode
        if ( ! preClassify )
            return;
        SetOctreeAlgorithm();
    } else if (item_no == (XtPointer)1) { //Use Pre-Classified Mode
        if ( preClassify )
            return;
        SetPreClassifyAlgorithm();
    }
    projPicturePtr->GetVolRenderPtr()->InvalidateVPData();
    
    if (XmToggleButtonGetState(wAutoDraw)||showing3dRender) {
        DoRender(w, NULL, NULL);
    }
}


// -------------------------------------------------------------------
void PltApp::DoLabelAxes(Widget, XtPointer, XtPointer) {
  labelAxes = (labelAxes ? false : true);
  XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
  DoExposeTransDA();
}


/*
// -------------------------------------------------------------------
void PltApp::WriteSWFData(const aString &filenamebase) {
  cout << "_in WriteSWFData:  filenamebase = " << filenamebase << endl;
#if defined(BL_VOLUMERENDER)
  VolRender *volRender = projPicturePtr->GetVolRenderPtr();
#endif
#if defined(BL_PARALLELVOLUMERENDER)
  PVolRender *volRender = projPicturePtr->GetVolRenderPtr();
#endif
  //projPicturePtr->AllocateSWFData();
  int iPaletteStart = pltPaletteptr->PaletteStart();
  int iPaletteEnd   = pltPaletteptr->PaletteEnd();
  int iBlackIndex   = pltPaletteptr->BlackIndex();
  int iWhiteIndex   = pltPaletteptr->WhiteIndex();
  int iColorSlots   = pltPaletteptr->ColorSlots();
  volRender->MakeSWFData(dataServicesPtr,
			 globalMin, globalMax,
			 currentDerived, 
			 iPaletteStart, iPaletteEnd,
			 iBlackIndex, iWhiteIndex,
			 iColorSlots);
  volRender->WriteSWFData(filenamebase);
}
*/


// -------------------------------------------------------------------
void PltApp::DoRender(Widget w, XtPointer, XtPointer) {
#if defined(BL_VOLUMERENDER) || defined(BL_PARALLELVOLUMERENDER)
  //XClearWindow(XtDisplay(wTransDA), XtWindow(wTransDA));
  //if( ! projPicturePtr->SWFDataAllocated()) {
    //projPicturePtr->AllocateSWFData();
  //}
  if (!showing3dRender) showing3dRender = true;
  Widget currentAutoDraw = (transDetached? wDAutoDraw : wAutoDraw);
#if defined(BL_VOLUMERENDER)
  VolRender *volRender = projPicturePtr->GetVolRenderPtr();
#endif
#if defined(BL_PARALLELVOLUMERENDER)
  PVolRender *volRender = projPicturePtr->GetVolRenderPtr();
#endif
  if( ! volRender->SWFDataValid()) {
    int iPaletteStart = pltPaletteptr->PaletteStart();
    int iPaletteEnd   = pltPaletteptr->PaletteEnd();
    int iBlackIndex   = pltPaletteptr->BlackIndex();
    int iWhiteIndex   = pltPaletteptr->WhiteIndex();
    int iColorSlots   = pltPaletteptr->ColorSlots();
    Real minUsing(amrPicturePtrArray[ZPLANE]->MinUsing());
    Real maxUsing(amrPicturePtrArray[ZPLANE]->MaxUsing());
    volRender->MakeSWFData(dataServicesPtr[currentFrame],
				minUsing, maxUsing,
				currentDerived, 
				iPaletteStart, iPaletteEnd,
				iBlackIndex, iWhiteIndex,
				iColorSlots);
  }
  if( ! volRender->VPDataValid()) {
    volRender->MakeVPData();
  }
  if( ! XmToggleButtonGetState(currentAutoDraw)) {
//    projPicturePtr->MakeBoxes();
  }
  projPicturePtr->MakePicture();
  projPicturePtr->DrawPicture();
#endif
}

// -------------------------------------------------------------------
void CBExposeTransDA(Widget, XtPointer client_data, XExposeEvent) {
  ((PltApp *) client_data)->DoExposeTransDA();
}
// -------------------------------------------------------------------
// -------------------------------------------------------------------
