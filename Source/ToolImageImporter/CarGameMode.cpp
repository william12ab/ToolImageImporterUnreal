#include "CarGameMode.h"
#include "VehicleController.h"
#include "CarHUD.h"
ACarGameMode::ACarGameMode(){
	DefaultPawnClass = AVehicleController::StaticClass();
	HUDClass = ACarHUD::StaticClass();
}
