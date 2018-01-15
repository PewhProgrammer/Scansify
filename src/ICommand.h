#pragma once
class ICommand
{
public:

	virtual bool verify() = 0;
	virtual void execute() = 0;
};

