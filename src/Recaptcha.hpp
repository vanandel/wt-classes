/*
 * wt-classes, utility classes used by Wt applications
 * Copyright (C) 2011 Boris Nagaev
 *
 * See the LICENSE file for terms of use.
 */

#ifndef WC_RECAPTCHA_HPP_
#define WC_RECAPTCHA_HPP_

#include <boost/system/error_code.hpp>

#include <Wt/WGlobal>

#include "AbstractCaptcha.hpp"
#include "config.hpp"

namespace Wt {

namespace Wc {

/** reCAPTCHA widget.
To use this class, you should provide keys for Recaptcha API.
These keys can be generated on http://www.google.com/recaptcha

\note Multiple instances of Recaptcha on same page are not allowed.

<h3>CSS</h3>
Text "reCAPTCHA" above the image: \c wc_recaptcha_title.

\ingroup protection
\ingroup bindings
*/
class Recaptcha : public AbstractCaptcha {
public:
    /** Constructor */
    Recaptcha(const std::string& public_key, const std::string& private_key,
              WContainerWidget* parent = 0);

    /** Destructor */
    ~Recaptcha();

    /** Set the type of the CAPTCHA to 'image' */
    void get_image();

    /** Set the type of the CAPTCHA to 'audio' */
    void get_audio();

    /** Enable or disable buttons.
    Buttons affected are "Update" button, "Get image" and "Get audio".

    According to reCAPTCHA rules, you must provide these buttons (AJAX version).
    Button "Get image" should be of CSS class "recaptcha_only_if_audio".
    Button "Get audio" should be of CSS class "recaptcha_only_if_image".
    */
    void set_buttons(bool enabled);

    void set_input(WFormWidget* input);

protected:
    void update_impl();
    void check_impl();

private:
    bool buttons_enabled_;
    Http::Client* http_;
    std::string public_key_;
    std::string private_key_;
    WFormWidget* input_;
    WFormWidget* response_field_;
    WFormWidget* challenge_field_;

    bool js() const;
    WContainerWidget* get_impl();
    void http_done(const boost::system::error_code& e,
                   const Http::Message& message);
    void add_buttons();
};

}

}

#endif

