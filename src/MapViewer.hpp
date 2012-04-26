/*
 * Copyright (C) 2011 Ramil Mintaev
 *
 * See the LICENSE.MapViewer file for terms of use.
 */

#ifndef WC_MAP_VIEWER_HPP_
#define WC_MAP_VIEWER_HPP_

#include <Wt/WGlobal>
#include <Wt/WCompositeWidget>
#include <Wt/WGoogleMap>
#include <Wt/WPoint>
#include <Wt/WSignal>
#include <Wt/WJavaScript>
#include <Wt/WEvent>

namespace Wt {

namespace Wc {

/** Map viewer.
It is intended to facilitate an introduction of
a dynamic map in any web page.

Main features:
 - It uses free content (<a href="http://openstreetmap.org/">OpenStreetMap</a>).
   Ajax version is based on <a href="http://openlayers.org">OpenLayers</a> API.
 - It supports HTML version.

Additional features:
 - coordinates can be set by a method,
 - map can be moved using buttons,
 - map can be moved using mouse (Ajax only),
 - map can be scaled (zoom in, zoom out).

\ingroup bindings
*/
class MapViewer : public Wt::WCompositeWidget {
public:
    /** A geographical coordinate (latitude/longitude) */
    typedef Wt::WGoogleMap::Coordinate Coordinate;

    /** Constructor */
    MapViewer(Wt::WContainerWidget* parent = 0);

    /** Destructor */
    ~MapViewer();

    /** Sets the map view to the given center */
    void set_center(const Coordinate& center);

    /** Sets the map view to the given center and zoom level */
    void set_center(const Coordinate& center, int zoom);

    /** Allows to pan to a new lonlat.
    If the new lonlat is in the current extent,
    the map will slide smoothly (if implemented js).
    */
    void pan_to(const Coordinate& center);

    /** Allows to top shift */
    void top_shift(double power = 1.0);

    /** Allows to bottom shift */
    void bottom_shift(double power = 1.0);

    /** Allows to left shift */
    void left_shift(double power = 1.0);

    /** Allows to right shift */
    void right_shift(double power = 1.0);

    /** Increments zoom level by one */
    void zoom_in();

    /** Decrements zoom level by one */
    void zoom_out();

    /** Zoom to a specific zoom level */
    void zoom_to(int level);

    /** The click event.
    Event signal emitted when a mouse key was clicked on this widget.
    */
    Signal<Coordinate>& clicked() {
        return clicked_;
    }

protected:
    /** Layer Constructor.
    A Layer is a data source -- information about how OpenLayers
    should request data and display it.
    */
    void add_osm_layer(const std::string& layer_var_name,
                       const std::string& param = "");

    /** Update map viewer */
    void update_impl();

    /** The click event.
    Event signal emitted when a mouse key was clicked on this widget.

    \note When JavaScript is disabled, the signal will never fire.
    */
    JSignal<Coordinate>& jclicked() {
        return jclicked_;
    }

    /** Removes a layer from the map.
    Removes a layer from the map by removing its visual element
    (the layer.div property), then removing it from the map's internal
    list of layers, setting the layer’s map property to null.
    */
    void remove_layer();

private:
    typedef std::pair<Coordinate, Coordinate> CoordinatePair;

    Wt::WContainerWidget* get_impl();
    bool js() const;

    WPoint w2t(const Coordinate& pos, int zoom) const;
    Coordinate t2w(const WPoint& pos, int zoom) const;
    CoordinatePair marginal_pic_coords(const WPoint& tile) const;
    CoordinatePair marginal_pic_coords(const Coordinate& pos) const;
    std::pair<int, int> tile_coord2tile_left_top(const Coordinate& pos);
    void views_map_in_html();
    WContainerWidget* get_html_control_panel();

    template <class Type> Type get_abs(Type val);
    double diff_between(double x, double y);
    Side get_side(int v);
    std::string store_jsv(const std::string& key,
                          const std::string& value) const;
    std::string get_stored_jsv(const std::string& key) const;
    std::string get_lonlat_jsc(const Coordinate& pos) const;
    void click_on(const WPoint& tile_xy,
                  const WMouseEvent::Coordinates& img_xy);
    void jclick_on(Coordinate pos);
    std::string set_js_listener_control_(const JSignal<Coordinate> &signal,
                                         const std::string& signal_name) const;
    void set_click_signal_();

    std::string map_name_;
    std::string layer_name_;
    int zoom_;
    CoordinatePair marginal_tile_coords_;
    Coordinate center_;
    WPoint xy_center_;
    Signal<Coordinate> clicked_;
    JSignal<Coordinate> jclicked_;

    /** Destroy the map.
    Note that if you are using an application which
    removes a container of the map from the DOM, you need to ensure
    that you destroy the map before this happens; otherwise, the page
    unload handler will fail because the DOM elements that destroy_map()
    wants to clean up will be gone.
    */
    void destroy_map();
};

}

}

#endif
