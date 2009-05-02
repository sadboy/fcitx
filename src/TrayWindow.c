#include "TrayWindow.h"

tray_win_t tray;
extern Display *dpy;

Bool CreateTrayWindow() {
    tray.window = XCreateSimpleWindow (dpy, DefaultRootWindow (dpy), \
            0, 0, 1, 1, 0, \
            BlackPixel (dpy, DefaultScreen (dpy)), \
            WhitePixel (dpy, DefaultScreen (dpy)));
    if (tray.window == (Window) NULL)
        return False;

    tray.xpm_attr.valuemask = XpmCloseness;
    tray.xpm_attr.closeness = 40000;

    if (XpmCreatePixmapFromData(dpy, tray.window,
				 inactive_xpm,
				 &tray.icon[INACTIVE_ICON],
				 &tray.icon_mask[INACTIVE_ICON],
				 &tray.xpm_attr) != XpmSuccess) {
        fprintf(stderr, "failed to get inactive icon image\n");
        return False;
    }

    if (XpmCreatePixmapFromData(dpy, tray.window,
				active_xpm,
				&tray.icon[ACTIVE_ICON],
				&tray.icon_mask[ACTIVE_ICON],
				&tray.xpm_attr) != XpmSuccess) {
        fprintf(stderr, "failed to get active icon image\n");
        return False;
    }

    /* GCs for copy and drawing on mask */
    XGCValues gv;
    gv.function = GXcopy;
    gv.foreground = BlackPixel(dpy, DefaultScreen(dpy));
    tray.gc = XCreateGC(dpy, tray.window, GCFunction|GCForeground, &gv);

    tray_init(dpy, tray.window);

    /* inital set up of shaped window */
    XCopyArea(dpy, tray.icon[INACTIVE_ICON], tray.window, tray.gc,
	      0, 0, tray.xpm_attr.width, tray.xpm_attr.height, 0, 0);

    XShapeCombineMask (dpy, tray.window, ShapeBounding, 0, 0,
		       tray.icon_mask[INACTIVE_ICON], ShapeSet);

    XSelectInput (dpy, tray.window, ExposureMask | KeyPressMask | \
            ButtonPressMask | ButtonReleaseMask | StructureNotifyMask \
            | EnterWindowMask | PointerMotionMask | LeaveWindowMask | VisibilityChangeMask);

    return True;
}

void DrawTrayWindow(int f_state) {
    XCopyArea(dpy, tray.icon[f_state], tray.window, tray.gc,
          0, 0, tray.xpm_attr.width, tray.xpm_attr.height, 0, 0);
}

void tray_win_deinit(tray_win_t *f_tray) {
    ;
}
