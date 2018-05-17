#include "Engine.hpp"
#include "B2Fabric.hpp"
#include "SFMLFabric.hpp"
#include "Injection/DependencyInjectionManager.hpp"

#include "PGGameMode.hpp"
#include "PGPlayerController.hpp"


using PGSimulationConfig = TSimulationModeFabric<SimulationMode, PGPlayerController, PGGameMode>;


int main()
{
	DependencyInjectionManager::SetFrontendFabric(SFMLFabric::Get());
	DependencyInjectionManager::SetPhysicsConfig (B2Fabric  ::Get());

	auto simulationFabric = PGSimulationConfig::Get();
	
	Engine engine;
	engine.SetPathToConfig("EngineConfig.txt");
	engine.SetSimulationFabric(std::move(simulationFabric));
	engine.SaveConfig();
	return engine.MainCycle();
}