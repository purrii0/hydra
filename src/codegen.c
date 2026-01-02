#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>

static void indent(FILE *out, int level) {
  for (int i = 0; i < level; i++)
    fprintf(out, "    ");
}

static const char *type_to_c(DataType t) {
  switch (t) {
  case TYPE_INT:
    return "int";
  case TYPE_CHAR:
    return "char";
  case TYPE_FLOAT:
    return "float";
  case TYPE_STRING:
    return "char*";
  case TYPE_VOID:
    return "void";
  default:
    return "int";
  }
}

static void emit_expr(Node *node, FILE *out) {
  switch (node->type) {

  case NODE_LITERAL:
  case NODE_IDENT:
    fprintf(out, "%s", node->value);
    break;

  case NODE_BINARY_OP:
    fprintf(out, "(");
    emit_expr(node->children[0], out);
    fprintf(out, " %s ", node->value);
    emit_expr(node->children[1], out);
    fprintf(out, ")");
    break;

  default:
    fprintf(stderr, "Unsupported expression node %d\n", node->type);
    exit(1);
  }
}

static void emit_node(Node *node, FILE *out, int indent_level) {
  switch (node->type) {

  case NODE_PROGRAM:
    fprintf(out, "#include <stdio.h>\n\n");
    for (int i = 0; i < node->child_count; i++)
      emit_node(node->children[i], out, 0);
    break;

  case NODE_PRINT:
    indent(out, indent_level);
    fprintf(out, "printf(\"%%d\\n\", ");
    emit_expr(node->children[0], out);
    fprintf(out, ");\n");
    break;

  case NODE_FUNC_DECL: {
    indent(out, indent_level);
    fprintf(out, "%s %s(", type_to_c(node->return_type), node->value);

    Node *params = node->children[0];
    for (int i = 0; i < params->child_count; i++) {
      Node *p = params->children[i];
      fprintf(out, "%s %s", type_to_c(p->return_type), p->value);
      if (i + 1 < params->child_count)
        fprintf(out, ", ");
    }

    fprintf(out, ")\n");
    emit_node(node->children[1], out, indent_level);
    fprintf(out, "\n");
    break;
  }

  case NODE_BLOCK:
    indent(out, indent_level);
    fprintf(out, "{\n");
    for (int i = 0; i < node->child_count; i++)
      emit_node(node->children[i], out, indent_level + 1);
    indent(out, indent_level);
    fprintf(out, "}\n");
    break;

  case NODE_VAR_DECL:
    indent(out, indent_level);
    fprintf(out, "%s %s", type_to_c(node->return_type), node->value);

    if (node->child_count > 0) {
      fprintf(out, " = ");
      emit_expr(node->children[0], out);
    }

    fprintf(out, ";\n");
    break;

  case NODE_ASSIGN:
    indent(out, indent_level);
    fprintf(out, "%s = ", node->value);
    emit_expr(node->children[0], out);
    fprintf(out, ";\n");
    break;

  case NODE_RETURN:
    indent(out, indent_level);
    fprintf(out, "return ");
    emit_expr(node->children[0], out);
    fprintf(out, ";\n");
    break;

  case NODE_IF:
    indent(out, indent_level);
    fprintf(out, "if (");
    emit_expr(node->children[0], out);
    fprintf(out, ")\n");
    emit_node(node->children[1], out, indent_level);

    if (node->child_count == 3) {
      indent(out, indent_level);
      fprintf(out, "else\n");
      emit_node(node->children[2], out, indent_level);
    }
    break;

  default:
    fprintf(stderr, "Unhandled node type %d\n", node->type);
    exit(1);
  }
}

void generate_c(Node *ast, const char *output_file) {
  FILE *out = fopen(output_file, "w");
  if (!out) {
    perror("fopen");
    exit(1);
  }

  emit_node(ast, out, 0);
  fclose(out);
}
