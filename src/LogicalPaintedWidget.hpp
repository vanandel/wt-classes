/*
 * wt-classes, utility classes used by Wt applications
 * Copyright (C) 2011 Boris Nagaev
 *
 * See the LICENSE file for terms of use.
 */

#ifndef WC_LOGICAL_PAINTED_WIDGET_HPP_
#define WC_LOGICAL_PAINTED_WIDGET_HPP_

#include <Wt/WGlobal>
#include <Wt/WPaintedWidget>
#include <Wt/WTransform>
#include <Wt/WRectF>

namespace Wt {

namespace Wc {

/** An extension of WPaintedWidget, related to logical coordinated

\attention Don't forget to set logical coordinates window (set_logical_window())
    and to set this window and a view port from paintEvent() method
    (use_logical()).

\ingroup util
*/

class LogicalPaintedWidget : public WPaintedWidget {
public:
    /** Constructor */
    LogicalPaintedWidget(WContainerWidget* parent = 0);

    /** Set the window bounding points in logical coordinates.
    \param window  The window.
    \param border Add 5% to each side of the rectangle.
    \param preserve_aspect  Whether width to height ratio should be preserved.
    Logical window defaults to null WRectF.
    */
    void set_logical_window(const WRectF& window, float border = 0.05,
                            bool preserve_aspect = true);

    /** Return the window bounding points in logical coordinates */
    const WRectF& logical_window() const {
        return logical_window_;
    }

    /** Return the window bounding points in device coordinates */
    const WRectF& logical_view_port() const {
        return logical_view_port_;
    }

    /** Apply mapping from logical to device to the painter.
    You MUST apply this method to the painter from paintEvent() method
    in order to use local coordinates in painting operations.

    This method sets logical_window() as a window and
    logical_view_port() as a view port.
    */
    void use_logical(WPainter& painter) const;

    /** Set this painter to use device coordinates.
    This may be useful while drawing a text.
    To draw a text avoiding issues with font scaling,
    call this method and temporary use device coordinates
    (actual coordinates can be calculated with logical2device()),
    and then call use_logical() to restore mapping from logical to device.
    */
    void use_device(WPainter& painter) const;

    /** Map the point from logical to device coordinates */
    WPointF logical2device(const WPointF& logical) const;

    /** Map the point from device to logical coordinates */
    WPointF device2logical(const WPointF& device) const;

    /** Get the matrix used for device-to-logical mapping */
    const WTransform& device2logical_matrix() const {
        return device2logical_;
    }

    /** Get the matrix used for logical-to-device mapping */
    const WTransform& logical2device_matrix() const {
        return logical2device_;
    }

    /** Return rectangle representing device coordinates */
    WRectF device_window() const;

    /** Return the rectangle with added borders.
    \param rect   Input rectangle.
    \param border A fraction of width/height to be added to each side.
    */
    static WRectF add_borders(const WRectF& rect, float border);

    /** Recalculate internal matrices.
    This is a convenience method, and is equivalent to:
    update_matrices(device_window(), preserve_aspect).
    */
    void update_matrices(bool preserve_aspect = true);

    /** Recalculate internal matrices.
    \param device           The size of device.
    \param preserve_aspect  Whether width to height ratio should be preserved.
    This method should be called after device size changed.
    */
    void update_matrices(const WRectF& device, bool preserve_aspect = true);

    /** Resizes the widget.
    This method calls the method of WPaintedWidget and
    update_matrices() with previously used preserve_aspect.
    */
    void resize(const WLength& width, const WLength& height);

protected:
    /** Resizes the widget.
    This method calls the method of WPaintedWidget and
    update_matrices() with previously used preserve_aspect.
    */
    void layoutSizeChanged(int width, int height);

private:
    WRectF logical_window_;
    WRectF logical_view_port_;
    WTransform device2logical_;
    WTransform logical2device_;
    bool preserve_aspect_;

    WRectF change_aspect(const WRectF& rect, const WRectF& master);
};

}

}

#endif

