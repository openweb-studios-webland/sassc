#include <iostream>
#include <string>
#include "node.hpp"

using std::string;
using std::stringstream;
using std::cout;
using std::endl;

namespace Sass {
  Node::Node() { }
  Node::Node(Node_Type _type) {
    type = _type;
  }
  Node::Node(Node_Type _type, Token& _token) {
    type = _type;
    token = _token;
  }
  void Node::push_child(const Node& node) {
    children.push_back(node);
  }
  void Node::push_opt_child(const Node& node) {
    opt_children.push_back(node);
  }
  
  void Node::dump(unsigned int depth) {
    switch (type) {
    case comment:
      for (int i = depth; i > 0; --i) cout << "  ";
      cout << string(token) << endl;
      break;
    case selector:
      cout << string(token);
      break;
    case value:
      cout << string(token);
      break;
    case property:
      cout << string(token) << ":";
      break;
    case values:
      for (int i = 0; i < children.size(); ++i)
        cout << " " << string(children[i].token);
      break;
    case rule:
      for (int i = depth; i > 0; --i) cout << "  ";
      children[0].dump(depth);
      children[1].dump(depth);
      cout << ";" << endl;
      break;
    case declarations:
      cout << " {" << endl;
      for (int i = 0; i < children.size(); ++i) {
        children[i].dump(depth + 1);
      }
      for (int i = 0; i < opt_children.size(); ++i) {
        opt_children[i].dump(depth + 1);
      }
      for (int i = depth; i > 0; --i) cout << "  ";
      cout << "}" << endl;
      break;
    case ruleset:
      for (int i = depth; i > 0; --i) cout << "  ";
      children[0].dump(depth);
      children[1].dump(depth);
      break;
    default: cout << "HUH?"; break;
    }
  }
  
  void Node::emit_expanded_css(stringstream& buf, string prefix) {
    switch (type) {
    case value:
    case selector:
      buf << string(token);
      break;
    case comment:
      buf << string(token) << endl;
      break;
    case property:
      buf << string(token) << ":";
      break;
    case values:
      for (int i = 0; i < children.size(); ++i)
        buf << " " << string(children[i].token);
      break;
    case rule:
      buf << "  ";
      children[0].emit_expanded_css(buf, prefix);
      children[1].emit_expanded_css(buf, prefix);
      buf << ";" << endl;
      break;
    case declarations:
      buf << " {" << endl;
      for (int i = 0; i < children.size(); ++i)
        children[i].emit_expanded_css(buf, prefix);
      buf << "}" << endl;
      for (int i = 0; i < opt_children.size(); ++i)
        opt_children[i].emit_expanded_css(buf, prefix);
      break;
    case ruleset:
      buf << prefix << " ";
      children[0].emit_expanded_css(buf, prefix);
      children[1].emit_expanded_css(buf, prefix + string(children[0].token));
      break;
    }
  }

}