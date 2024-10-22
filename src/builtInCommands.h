#ifndef BUILTINCOMMANDS_H
#define BUILTINCOMMANDS_H

#include <string>
#include <vector>

void pwd();
void cd(const vector<string>& args);
void echo(vector<string>& args);
#endif
