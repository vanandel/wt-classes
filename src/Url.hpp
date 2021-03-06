/*
 * wt-classes, utility classes used by Wt applications
 * Copyright (C) 2011 Boris Nagaev
 *
 * See the LICENSE file for terms of use.
 */

#ifndef WC_URL_HPP_
#define WC_URL_HPP_

#include "config.hpp"

#include <map>
#include <string>
#include <ostream>
#include <boost/function.hpp>
#include <boost/any.hpp>

#include <Wt/WGlobal>
#include <Wt/WObject>
#include <Wt/WSignal>
#include <Wt/WDate>
#ifdef WC_HAVE_WLINK
#include <Wt/WLink>
#endif

#include "global.hpp"

namespace Wt {

namespace Wc {

/** Namespace for dealing with internal paths.

\ingroup url
*/
namespace url {

/** \defgroup url URL-related
Classes for URL rules.
This classes allow you to keep all url-related code in one place.

\note Before reading the rest of this documentation,
    read about internal paths in the Wt documentation.

See SiteMapGenerator for information on how to generate sitemap.

<h3>node</h3>

Every internal path is represented as a Node in a tree.
For example, internal path <tt>/about/license</tt>
is a child of <tt>/about/</tt>.

To create a child node, just pass parent node to the constructor of a child.
Constructors of nodes of some types take additional arguments.

Each node has a \ref Node::value() "value", the string,
representing the last part of that internal path.
For example, the value of "/about/license" is "license".

There are several types of node:
 - PredefinedNode (value is constant),
 - IntegerNode (value represents an integer),
 - StringNode (value represents a string),
 - Parser (value is an empty string; is the root of that tree of nodes).

IntegerNode and StringNode are mutable.
Their value (integer or string) can be changed.
This occurs while \ref Parser::open() "url opening".
Parser sets values of all nodes, involved into internal path parsed.
You can get the value using StringNode::string() or IntegerNode::integer(),
or general method Node::value().
To set the value (to get WLink's involving it), use StringNode::set_string()
of IntegerNode::set_integer_value().

After the creation of nodes, connect their \ref Node::opened() "opened()"
signal to appropriate slots.
For example, let internal path "/user/" be the list of users.
In this case, connect its opened() signal to the function,
displaying the list of users.
It is beyond this documentation, how to write this function.

If you do not want to spend memory on Node::opened() signal in all nodes,
you can connect all you code to Parser::child_opened() signal.
This signal is emitted with selected Node.

Third option is to use Parser::connect. It does not create any Signal's at all.
But it does not keep track deletion of target.

You can use any combination of these methods.
Order of calling: Node::opened(), then Parser::child_opened(),
and finally handlers passed to Parser::connect.

When you need the internal path, use Node::link() method.
There are methods get_link() in IntegerNode and StringNode, setting
the value and returning the link at once.
You can simulate the opening of an internal path,
using Node::open() method.

<h3>parser</h3>

Parser is the root in internal path tree.
It corresponds to the internal path <tt>/</tt> (main page of an application).
Nodes of other type are direct or indirect children of Parser node.

All created nodes should be bound to session.
You can create your own class, inherited from Parser,
which would create all child nodes, and bind an instance
of that class to an instance of WApplication.
Nodes will be needed to get internal paths, e.g. for WAnchor.
So it is better to provide public access to them.

You should connect wApp->internalPathChanged()
to Parser::open() to make it aware of internal path changes.

In the end of the constructor of WApplication's descendant
(or in other function, creating application for WServer)
pass internalPath() to Parser::open() to process initial internal path
(internal path, being set while application creation).

<h3>Short example</h3>

\code
Parser* parser = new Parser(wApp);
wApp->internalPathChanged().connect(parser, &Parser::open);
PredefinedNode* users = new PredefinedNode("user", parser);
users->opened().connect(...);        // show user list for "/user/"
IntegerNode* user_profile = new IntegerNode(users);
user_profile->opened().connect(...); // show user profile for "/user/xxx"
\endcode

<h3>Detailed example</h3>

In the example bellow, main features of this module are demonstrated:
 - tree of internal path nodes,
 - connection of Node::opened() to function, displaying something,
 - connection of Parser::child_opened() to function, which accepts Node.
 - connection to node handler using Parser::connect.
 - Parser is also a Node, it corresponds to the main page,
 - use of Parser::open(),
 - referring internal paths.

It is a model of content and user oriented site.

<tt>/about/</tt> represents a list of available articles.
Each article is identified with a string.
Article page is <tt>/about/article-name</tt>.
These two nodes are connected using Node::opened() method <b>[a]</b>.

The list of users is shown in <tt>/user</tt>.
It is shown as a "file", not as a "directory"
to demonstrate Node::SlashStrategy feature.
This node (\c users_ ) is connected through Parser::connect <b>[b]</b>.

Each user has integer identifier.
User profile is shown in internal path like <tt>/user/123</tt>.
This node (\c user_profile_ ) is connected by Parser::child_opened() <b>[c]</b>.

The main page contain references to the list of articles and the list of users.
Every page has a reference to the main page on top.

User identifiers and article names are generated randomly.

\include examples/url.cpp

*/

/** Part of URL.
This class represents a node in url parsing tree.

For example, for internal path \c "/user/1", there are two nodes:
 - \c "user", is represented with PredefinedNode;
 - \c "1", is represented with IntegerNode.

\ingroup url
*/
class Node : public WObject {
public:
    /** When to add a slash after the node */
    enum SlashStrategy {
        IF_NOT_LAST, /**< Add only if the node is not last node in the URL */
        IF_HAS_CHILD, /**< Add only if the node has at least one child object */
        ALWAYS, /**< Always add */
        DEFAULT = IF_HAS_CHILD /**< Default strategy */
    };

    /** Constructor.
    \param parent Node (Parser or other Node)
    */
    Node(WObject* parent = 0);

    /** Destructor */
    ~Node();

    /** Return if this part of an url meets the node */
    virtual bool meet(const std::string& part) const = 0;

    /** Get value */
    const std::string& value() const {
        return value_;
    }

    /** Add this node to output stream.
    \param path Output stream.
    \param is_last If the node is the last node in the URL.
    Add to the stream representation of this node and '/',
    depending on is_last and slash().
    */
    void write_to(std::ostream& path, bool is_last = false) const;

    /** Write all parents and this node to the stream.
    If root is specified, then path is started from it,
    not from normal root.
    Anyway, path is started with '/'.
    */
    void write_all_to(std::ostream& path, Node* root = 0) const;

    /** Return full internal path to this node */
    std::string full_path() const;

#ifdef WC_HAVE_WLINK
    /** Return an internal path link to this node */
    WLink link() const;
#endif

    /** Return parent of type Node or 0 */
    Node* node_parent() const;

    /** Return parser or 0 */
    Parser* parser() const;

    /** Emit signals.
    \param change_path Whether internal path of wApp should be changed.
    */
    void open(bool change_path = true);

    /** Signal emitted when url is opened.
    This signal is created lazily.
    \see Parser::child_opened(), Parser::connect
    */
    Signal<>& opened();

    /** Get slash strategy */
    SlashStrategy slash_strategy() const {
        return slash_strategy_;
    }

    /** Get slash strategy.
    By default, DEFAULT slash strategy is applied.
    */
    void set_slash_strategy(SlashStrategy slash_strategy) {
        slash_strategy_ = slash_strategy;
    }

protected:
    /** Set value.
    If check is true, the value is checked using meet() before setting.
    */
    void set_value(const std::string& v, bool check = false);

private:
    Signal<>* opened_;
    std::string value_;
    SlashStrategy slash_strategy_ : 8;

    friend class Parser;
    friend class SiteMapGenerator;
};

/** Predefined part of an URL.

\ingroup url
*/
class PredefinedNode : public Node {
public:
    /** Constructor */
    PredefinedNode(const std::string& predefined, WObject* parent = 0);

    bool meet(const std::string& part) const;

    /** Get predefined string */
    const std::string& predefined() const {
        return predefined_;
    }

private:
    const std::string predefined_;
};

/** Part of an URL, represented with arbitrary integer (long long).
\note The value is temporary and used for
      generation and parsing arbitrary paths.

\ingroup url
*/
class IntegerNode : public Node {
public:
    /** Constructor */
    IntegerNode(WObject* parent = 0);

    bool meet(const std::string& part) const;

    /** Get integer value.
    \attention Throws boost::bad_lexical_cast exception if value is not integer.
    */
    long long integer() const;

    /** Set value */
    void set_integer_value(long long v);

    /** Set value and return full internal path to this node */
    std::string get_full_path(long long v);

#ifdef WC_HAVE_WLINK
    /** Set value and return an internal path link to this node */
    WLink get_link(long long v);
#endif
};

/** Part of an URL, represented with arbitrary string.
\note The value is temporary and used for
      generation and parsing arbitrary paths.

\ingroup url
*/
class StringNode : public Node {
public:
    /** Constructor */
    StringNode(WObject* parent = 0);

    bool meet(const std::string& part) const;

    /** Set value */
    void set_string(const std::string& string);

    /** Get value */
    const std::string& string() const;

    /** Set value and return full internal path to this node */
    std::string get_full_path(const std::string& v);

#ifdef WC_HAVE_WLINK
    /** Set value and return an internal path link to this node */
    WLink get_link(const std::string& v);
#endif
};

/** Path parser.
In addition, it is the root url node ("index" page, internal path "/").
All other nodes should be children (direct or indirect) of this node.

This object should be bound to session.

\ingroup url
*/
class Parser : public Node {
public:
    /** Constructor */
    Parser(WObject* parent = 0);

    bool meet(const std::string& part) const;

    /** Parse the internal path.
    Set values to all non-predefined nodes and return last node.
    If node is undefined, returns 0.
    */
    Node* parse(const std::string& path);

#ifndef DOXYGEN_ONLY
    void open(Node* node);
#endif

    /** Parse the internal path and open corresponding path node.
    This method does not change URL in address line
    (as if Node::open() is called with change_path = false).
    */
    void open(const std::string& path);

#ifdef WC_HAVE_WLINK
    /** Parse the internal path and open corresponding path node.
    This is an overloaded method for convenience.
    */
    void open(const WLink& internal_path);
#endif

    /** Signal emitted when wrong internal path is opened */
    Signal<>& error404() {
        return error404_;
    }

    /** Signal emitted when url is opened.
    \see Parser::connect, Node::opened()
    */
    Signal<Node*>& child_opened() {
        return child_opened_;
    }

    /** Add the handler for the node.
    \warning Unlike signals, this method do not check that slot is not removed.
    \see Node::opened(), Parser::child_opened()
    */
    void connect(Node* child, boost::function<void()> handler);

    /** Disconnect all handlers from the node */
    void disconnect(Node* child);

private:
    typedef boost::function<void()> Handler;
    typedef std::multimap<Node*, Handler> Handlers;

    Signal<> error404_;
    Signal<Node*> child_opened_;
    Handlers handlers_;
};

/** Generator of sitemap from Node objects.

See http://www.sitemaps.org/protocol.html
for more information about Sitemaps XML format.

\ingroup url
*/
class SiteMapGenerator {
public:
    /** How frequently the page is likely to change */
    enum ChangeFreq {
        ALWAYS, /**< For documents changing every moment */
        HOURLY, /**< Hourly */
        DAILY, /**< Daily */
        WEEKLY, /**< Weekly */
        MONTHLY, /**< Monthly */
        YEARLY, /**< Yearly */
        NEVER /**< For archive pages */
    };

    /** Properties of location */
    struct UrlParams {
        /** The date of last modification of the page */
        WDate lastmod;

        /** How frequently the page is likely to change */
        ChangeFreq changefreq;

        /** The priority of this URL relative to other URLs on your site.
        Defaults to 0.5.
        */
        float priority;
    };

    /** Constructor.
    \param root Root node, whose family will be presented in sitemap.
        This needs not to be general root (parser),
        you can pass any node here.
        Location will be concatenated from base_loc() and
        \ref Node::write_all_to "part of path beginning from root".
        (see example url-sitemap.cpp, "Sitemap of /about/").
    */
    SiteMapGenerator(Node* root);

    /** Stream sitemap */
    void generate(std::ostream& out) const;

    /** Return root node whose family will be presented in sitemap */
    Node* root() const {
        return root_;
    }

    /** Set root node whose family will be presented in sitemap */
    void set_root(Node* root) {
        root_ = root;
    }

    /** Base location */
    const std::string& base_loc() const {
        return base_loc_;
    }

    /** Set base location.
    Should not end with '/'.

    Defaults to urlScheme() + "://" + hostName() + path/to/root.
    */
    void set_base_loc(const std::string& base_loc) {
        base_loc_ = base_loc;
    }

    /** Get default params passed to node_handler() */
    const UrlParams& default_params() const {
        return default_params_;
    }

    /** Set default params passed to node_handler() */
    void set_default_params(const UrlParams& default_params) {
        default_params_ = default_params;
    }

protected:
    /** Function with one argument of type boost::any */
    typedef boost::function<void(boost::any)> AnyCaller;

    /** Call callback for each possible value of variable nodes.

    You should implement this function if you are using nodes like
    IntegerNode or StringNode.
    Node passed to this function is guaranteed not to be of
    class PredefinedNode or Parser or class inherited from them.

    For each possible value of node passed you should call callback function.
    Only arguments std::string and int are currently supported by the callback.
    Other type will cause bad_any_cast exception thrown.
    Each value passed to callback is checked with Node::meet().
    Values that don't meet, will be skipped.

    Default implementation does nothing, which is enough for
    PredefinedNode-only node trees.
    */
    virtual void for_each_value(Node* node, const AnyCaller& callback) const;

    /** Return if this location is present in sitemap and change its params.

    Default implementation returns true.
    */
    virtual bool node_handler(Node* node, UrlParams& params) const;

private:
    Node* root_;
    std::string base_loc_;
    UrlParams default_params_;

    void dig_node(Node* node, std::ostream& out) const;
    void visit_node(Node* node, std::ostream& out, boost::any value) const;
};

}

}

}

#endif

