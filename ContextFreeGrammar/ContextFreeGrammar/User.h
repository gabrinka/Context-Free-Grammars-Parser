#pragma once
#include <iostream>
#include <string>
#include "Grammar.h"
#include "PushdownAuto.h"


void run();
void command(string input,Grammar& first,Grammar& second,PushdownAuto& stackf,PushdownAuto& stacks);
