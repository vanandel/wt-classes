/*
 * wt-classes, utility classes used by Wt applications
 * Copyright (C) 2011 Boris Nagaev
 *
 * See the LICENSE file for terms of use.
 */

#ifndef WC_COUNTDOWN_HPP_
#define WC_COUNTDOWN_HPP_

#include <Wt/WGlobal>
#include <Wt/WContainerWidget>
#include <Wt/WJavaScript>
#include <Wt/WString>
#include <Wt/WDateTime>

#include "TimeDuration.hpp"

namespace Wt {

namespace Wc {

/** A countdown (or countup) to a given time.
The widget is implemented as a div,
displayed inline (see setInline(true)).

For JavaScript, <a href="http://keith-wood.name/countdown.html">
jquery countdown plugin</a> is used.
Currently, only compact form of jquery countdown is used.

For HTML version, countdown is displayed by C++.
You may use WTimer to update the page periodically.

By default, the widget counts up from now.

\include examples/countdown.cpp

\ingroup time
*/
class Countdown : public WContainerWidget {
public:
    /** Constructor */
    Countdown(WContainerWidget* parent = 0);

    /** Set the time to count up from.
    \note Setting \c since or \c until, you invalidate another one.
    */
    void set_since(const WDateTime& since);

    /** Set the time duration, relative to the current time, to count up from.
    \note Setting \c since or \c until, you invalidate another one.
    */
    void set_since(const td::TimeDuration& since);

    /** Get the time to count up from.
    \note Setting \c since or \c until, you invalidate another one.
    */
    const Wt::WDateTime& since() const {
        return since_;
    }

    /** Set the time to count down to.
    \note Setting \c since or \c until, you invalidate another one.
    */
    void set_until(const WDateTime& until);

    /** Set the time duration, relative to the current time, to count down to.
    \note Setting \c since or \c until, you invalidate another one.
    */
    void set_until(const td::TimeDuration& until);

    /** Get the time to count down to.
    \note Setting \c since or \c until, you invalidate another one.
    */
    const Wt::WDateTime& until() const {
        return until_;
    }

    /** Set the format for the countdown display.
    The format string may contain following characters (in order),
    indicating the periods to display:
     - 'Y' for years,
     - 'O' for months,
     - 'W' for weeks,
     - 'D' for days,
     - 'H' for hours,
     - 'M' for minutes,
     - 'S' for seconds.

    Lowercase character mean, that the period is displayed only if non-zero.
    If one optional period is shown, all the ones after that are also shown.
    */
    void set_format(const std::string& format = "dHMS");

    /** Get the format for the countdown display */
    const std::string format() const {
        return format_;
    }

    /** Set the separator between the various parts of the countdown time */
    void set_time_separator(const std::string& time_separator = ":");

    /** Get the separator between the various parts of the countdown time */
    const std::string time_separator() const {
        return time_separator_;
    }

    /** Low-level method to change settings of jquery countdown.
    \param name Name of the setting. Is always stringified by the method.
    \param value Value of the setting.
    \param stringify_value If the value should be stringified by the method.
    For HTML-version, does nothing.
    */
    void change(const std::string& name, const std::string& value,
                bool stringify_value = false);

    /** Return currently displayed text.
    The countdown displays zeros if the time has passed or
    has not yet arrived.
    */
    std::string current_text() const;

    /** Return currently displayed time duration.
    The countdown displays zeros if the time has passed or
    has not yet arrived.
    */
    td::TimeDuration current_duration() const;

private:
    class View;

    View* view_;
    bool js_;
    Wt::WDateTime since_;
    Wt::WDateTime until_;
    std::string format_;
    std::string time_separator_;

    static std::string duration_for_js(const td::TimeDuration& duration);
    void apply_js(const std::string& args);
    void update_view();
};

}

}

#endif

