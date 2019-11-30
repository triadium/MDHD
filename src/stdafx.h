// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#endif

#ifdef __cplusplus
#include <PlayrixEngine.h>

#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <Game/Collider.h>
#include <Game/Material.h>
#include <Game/View.h>
#include <Game/Object.h>
#include <Game/Processor.h>
#include <Game/Scene.h>

#include "Game/Constants.h"
#include "Game/Target.h"
#include "Game/TargetSpawner.h"
#include "Game/Bullet.h"
#include "Game/Gun.h"
#include "Game/StageTimeCountdown.h"
#include "Game/EggBomb.h"
#include "Game/DuckTarget.h"
#include "Game/FishTarget.h"
#include "Game/AnchovyTarget.h"
#endif
