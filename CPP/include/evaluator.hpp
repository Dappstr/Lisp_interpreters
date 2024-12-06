#pragma once

#include "ast.hpp"
#include "environment.hpp"

Value eval_atom(const Atom_Node &atom, Environment &env);
Value eval_list(const List_Node &list, Environment &env);
Value evaluate(const AST_Node &node, Environment &env);
