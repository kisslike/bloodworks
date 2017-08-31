#include "cTools.h"

#include <stdio.h>
#include <fstream>
#include <streambuf>
#include <SDL.h>
#include <algorithm>

#ifdef WINDOWS
#undef APIENTRY
#include "windows.h"
#endif

#ifdef _WIN32
#include "StackWalker.h"

class StackWalkerToConsole : public StackWalker
{
protected:
	virtual void OnOutput(LPCSTR szText)
	{
		printf("%s", szText);
	}
public:
	StackWalkerToConsole() : StackWalker(RetrieveNone)
	{

	}
	void print()
	{
		printf("\n\nStack Trace Begin\n--------------------\n");
		StackWalker::ShowCallstack();
		printf("\n--------------------\nStack Trace End\n");
	}

	void printExceptionStack(EXCEPTION_POINTERS* pExp)
	{
		printf("\n\nException Stack Trace Begin\n--------------------\n");
		ShowCallstack(GetCurrentThread(), pExp->ContextRecord);
		printf("\n--------------------\nException Stack Trace End\n");
	}
} stack;

#endif

float angleDiff(float a, float b)
{
	float diff = a - b;
	if (diff > pi)
	{
		diff -= pi_2;
	}
	else if (diff < -pi)
	{
		diff += pi_2;
	}
	return diff;
}

float approachAngle(float moveAngle, float wantedAngle, float rotation)
{
	if (wantedAngle != moveAngle)
	{
		float diff = angleDiff(moveAngle, wantedAngle);

		if (diff < -rotation)
		{
			moveAngle += rotation;
		}
		else if (diff > rotation)
		{
			moveAngle -= rotation;
		}
		else
		{
			moveAngle = wantedAngle;
		}
	}

	return moveAngle;
}

void sleepMS(int ms) 
{
#ifdef WINDOWS
	Sleep(ms);
#else
	SDL_Delay(ms);
#endif
}


int randInt()
{
	return rand();
}

float randFloat()
{
	return randInt() / (float)RAND_MAX;
}

bool randBool()
{
	return (randInt() & 1) == 0;
}

int randInt(int limit)
{
	return randInt() % limit;
}

int randInt(int begin, int end)
{
	return randInt() % (end - begin + 1) + begin;
}

float randFloat(float limit)
{
	return randFloat() * limit;
}

float randFloat(float begin, float end)
{
	return randFloat() * (end - begin) + begin;
}

void printStack()
{
#ifdef _WIN32
	stack.print();
#endif
}

void printExceptionStack(void* pExp)
{
#ifdef _WIN32
	stack.printExceptionStack((EXCEPTION_POINTERS*)pExp);
#endif
}

void doBreak()
{
#ifdef _WIN32
	__debugbreak();
#endif
}