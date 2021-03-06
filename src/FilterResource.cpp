/*
 * wt-classes, utility classes used by Wt applications
 * Copyright (C) 2011 Boris Nagaev
 *
 * See the LICENSE file for terms of use.
 */

#define BOOST_FILESYSTEM_VERSION 3

#include <cstdio>
#include <boost/filesystem.hpp>

#include <Wt/WApplication>
#include <Wt/WLogger>

#include "FilterResource.hpp"
#include "util.hpp"

namespace Wt {

namespace Wc {

FilterResource::FilterResource(const WString& cmd, WObject* parent):
    WFileResource("text/plain", "file.txt", parent), cmd_(cmd)
{ }

FilterResource::~FilterResource() {
    beingDeleted();
    remove(output_file_.c_str());
}

void FilterResource::handleRequest(const Http::Request& request,
                                   Http::Response& response) {
    using namespace boost::filesystem;
    mutex_.lock();
    if (output_file_.empty()) {
        std::string input_file = unique_filename();
        output_file_ = unique_filename();
        std::ofstream input(input_file.c_str());
        if (input.is_open()) {
            try {
                write_input(input);
                input.close();
                Wt::WString cmd = cmd_;
                system(cmd.arg(input_file).arg(output_file_).toUTF8().c_str());
                if (exists(output_file_)) {
                    setFileName(output_file_);
                }
            } catch (std::exception& e) {
                wApp->log("warning") << "FilterResource: " << e.what();
            } catch (...) {
                wApp->log("warning") << "FilterResource: error";
            }
        }
        remove(input_file.c_str());
    }
    mutex_.unlock();
    WFileResource::handleRequest(request, response);
}

void FilterResource::update(bool cal_set_changed) {
    mutex_.lock();
    if (!output_file_.empty()) {
        remove(output_file_.c_str());
        output_file_ = "";
    }
    mutex_.unlock();
    if (cal_set_changed) {
        setChanged();
    }
}

}

}

