/*
 * wt-classes, utility classes used by Wt applications
 * Copyright (C) 2011 Boris Nagaev
 *
 * See the LICENSE file for terms of use.
 */

#define BOOST_FILESYSTEM_VERSION 3

#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WServer>

#include "util.hpp"

namespace Wt {

namespace Wc {

void post(const boost::function<void()>& func, WApplication* app) {
    if (!app) {
        app = wApp;
    }
    WServer* server = app->environment().server();
    server->post(app->sessionId(), func);
}

boost::function<void()> bound_post(boost::function<void()> func) {
    return boost::bind(post, func, wApp);
}

void updates_trigger() {
    wApp->triggerUpdate();
}

void updates_poster(WServer* server, WApplication* app) {
    server->post(app->sessionId(), updates_trigger);
}

std::string unique_filename() {
    using namespace boost::filesystem;
    const char* const model = "wt-classes-%%%%-%%%%-%%%%-%%%%";
    return unique_path(temp_directory_path() / model).string();
}

std::string stringify(const std::string& value) {
    std::string copy(value);
    boost::replace_all(copy, "\\", "\\\\");
    boost::replace_all(copy, "\"", "\\\"");
    return "\"" + copy + "\"";
}

std::string config_value(const std::string& name, const std::string& def) {
    std::string value = def;
    if (wApp) {
        wApp->readConfigurationProperty(name, value);
    }
    return value;
}

}

}

