// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableActor.h"

bool AUsableActor::TryBeginUse()
{
	if (CanBeginUse())
	{
		return BeginUse();
	}
	return false;
}

bool AUsableActor::TryStopUse()
{
	if (CanStopUse())
	{
		return StopUse();
	}
	return false;
}

bool AUsableActor::IsInUse()
{
	return false;
}

bool AUsableActor::CanBeginUse()
{
	return true;
}

bool AUsableActor::BeginUse()
{
	return false;
}

bool AUsableActor::CanStopUse()
{
	return true;
}

bool AUsableActor::StopUse()
{
	return false;
}
