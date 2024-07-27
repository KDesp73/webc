#include "webc-actions.h"

WEBCAPI void WEBC_CleanRoute(Route* route)
{
    free(route->path);
    free(route->buffer);
    free(route);
}

WEBCAPI void WEBC_CleanTree(Tree tree)
{
    for(size_t i = 0; i < tree.count; ++i){
        WEBC_CleanRoute(tree.routes[i]);
    }
    free(tree.routes);
}

WEBCAPI Route* WEBC_MakeRoute(Cstr path, char* buffer)
{
    Route* route = (Route*) malloc(sizeof(Route));
    route->path = (char*) malloc(strlen(path) + 1);
    strcpy(route->path, path);
    route->buffer = (char*) malloc(strlen(buffer) + 1);
    strcpy(route->buffer, buffer);

    free(buffer);

    return route;
}

WEBCAPI Tree WEBC_MakeTree(Cstr root, Route* first, ...)
{
    Tree result = {0};
    result.root = root;
    result.count = 0;
    if(first == NULL) return result;

    va_list args;
    va_start(args, first);
    result.count++;
    for (Route* next = va_arg(args, Route*); next != NULL; next = va_arg(args, Route*)) {
        result.count++;
    }
    va_end(args);

    result.count = result.count;
    result.routes = (Route**) malloc(sizeof(result.routes[0]) * result.count);
    if (result.routes == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }

    va_start(args, first);
    result.routes[0] = first;
    for (size_t i = 1; i < result.count; ++i) {
        Route* next = va_arg(args, Route*);
        result.routes[i]= next;
    }
    va_end(args);

    return result; 
}

WEBCAPI Tree WEBC_AllocTree(Cstr root, size_t capacity)
{
    Tree tree = {0};
    tree.root = (char*) malloc(strlen(root));
    strcpy((char*) tree.root, root);
    tree.capacity = capacity;
    tree.routes = (Route**) malloc(sizeof(tree.routes[0]) * capacity);

    if (tree.routes == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }
    
    return tree;
}


WEBCAPI void WEBC_AddRoute(Tree* tree, Route* route)
{
    if(tree->count >= tree->capacity) {
        ERRO("Tree is at max capacity");
        return;
    }

    tree->routes[tree->count++] = route;
}

