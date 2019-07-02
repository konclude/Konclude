/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CConfigDependedCalculationEnvironmentFactory.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Calculation {


				CConfigDependedCalculationEnvironmentFactory::CConfigDependedCalculationEnvironmentFactory(CTaskHandleAlgorithmBuilder* taskHandleAlgBuilder) {
					mTaskHandleAlgBuilder = taskHandleAlgBuilder;
				}


				CConfigDependedCalculationEnvironmentFactory::~CConfigDependedCalculationEnvironmentFactory() {
				}


				CCalculationEnviroment *CConfigDependedCalculationEnvironmentFactory::createCalculationContext(CConfigurationProvider* configProvider) {
					CCalculationEnviroment *context = nullptr;

					CConfigurationBase* config = nullptr;
					if (configProvider) {
						config = configProvider->getCurrentConfiguration();
					}


					CWatchDog *watchDog = 0;


					bool configErrorFlag = false;

					bool taskCalcManager = false;


					QString calcManagerString = CConfigDataReader::readConfigString(config,"Konclude.Execution.CalculationManager",QString(),&configErrorFlag);
					if (!configErrorFlag) {
						if (calcManagerString.isEmpty() || calcManagerString == "Konclude.Calculation.Calculator.ConcurrentTaskCalculationManager") {
							taskCalcManager = true;
						} else {
							taskCalcManager = true;
						}
					} else {
						taskCalcManager = true;
					}

					if (taskCalcManager) {
						CConcurrentTaskCalculationEnvironment *taskContext = new CConcurrentTaskCalculationEnvironment();

						qint64 taskProcessorCount = 1;

						QString taskProcessorCountString = CConfigDataReader::readConfigString(config,"Konclude.Calculation.ProcessorCount",QString(),&configErrorFlag);
						if (!configErrorFlag) {
							if (taskProcessorCountString == "AUTO") {
								taskProcessorCount = CThread::idealThreadCount();
							}
							qint64 convertedTaskProcessorCount = taskProcessorCountString.toInt(&configErrorFlag);
							if (configErrorFlag) {
								taskProcessorCount = convertedTaskProcessorCount;
							}
						}


						CCentralizedAllocationConfigProvidedDependendLimitation* allocLimitation = new CCentralizedAllocationConfigProvidedDependendLimitation(configProvider,"Konclude.Calculation.Memory");
						taskContext->setAllocationLimitation(allocLimitation);

						CSatisfiableCalculationTaskJobCallbackExecuter* callbackExecuter = new CSatisfiableCalculationTaskJobCallbackExecuter();
						taskContext->initCallbackExecuter(callbackExecuter);
						CSatisfiableCalculationTaskStatusPropagator* taskStatusPropagator = new CSatisfiableCalculationTaskStatusPropagator();
						taskContext->initStatusPropagator(taskStatusPropagator);

						if (taskProcessorCount <= 1) {
							CTaskHandleAlgorithm* taskHandleAlg = mTaskHandleAlgBuilder->createTaskHandleAlgorithm();
							CConsiderateMemoryPoolProvider* memProv = new CNewCentralizedLimitedAllocationMemoryPoolProvider(allocLimitation->getLimitator());
							CSingleThreadTaskProcessorUnit* singlePocessUnit = new CSingleThreadTaskProcessorUnit(taskHandleAlg,memProv);
							singlePocessUnit->startProcessing();
							taskContext->initSingleTaskProcessor(singlePocessUnit);
							singlePocessUnit->installCallbackExecuter(callbackExecuter);
							singlePocessUnit->installStatusPropagator(taskStatusPropagator);
						} else {
							CTaskHandleAlgorithm* completorTaskHandleAlg = mTaskHandleAlgBuilder->createTaskHandleAlgorithm();
							CTaskHandleAlgorithm* schedulerTaskHandleAlg = mTaskHandleAlgBuilder->createTaskHandleAlgorithm();
							CConsiderateMemoryPoolProvider* compMemProv = new CNewCentralizedLimitedAllocationMemoryPoolProvider(allocLimitation->getLimitator());
							CConsiderateMemoryPoolProvider* schedMemProv = new CNewCentralizedLimitedAllocationMemoryPoolProvider(allocLimitation->getLimitator());
							CTaskProcessorCompletorThread* completorUnit = new CTaskProcessorCompletorThread(completorTaskHandleAlg,compMemProv);
							CTaskProcessorSchedulerThread* schedulerUnit = new CTaskProcessorSchedulerThread(schedulerTaskHandleAlg,completorUnit,schedMemProv);
							completorUnit->installScheduler(schedulerUnit);
							schedulerUnit->installScheduler(schedulerUnit);
							completorUnit->installCallbackExecuter(callbackExecuter);
							schedulerUnit->installCallbackExecuter(callbackExecuter);
							completorUnit->installStatusPropagator(taskStatusPropagator);
							schedulerUnit->installStatusPropagator(taskStatusPropagator);
							completorUnit->startProcessing();
							schedulerUnit->startProcessing();
							taskContext->initMultiTaskProcessor(schedulerUnit,completorUnit);

							while (taskProcessorCount-- > 2) {
								CTaskHandleAlgorithm* taskHandleAlg = mTaskHandleAlgBuilder->createTaskHandleAlgorithm();
								CConsiderateMemoryPoolProvider* memProv = new CNewCentralizedLimitedAllocationMemoryPoolProvider(allocLimitation->getLimitator());
								CTaskProcessorThread* taskProcessor = new CTaskProcessorThread(taskHandleAlg,completorUnit,memProv);
								taskProcessor->installScheduler(schedulerUnit);
								taskProcessor->installCallbackExecuter(callbackExecuter);
								taskProcessor->installStatusPropagator(taskStatusPropagator);
								taskProcessor->startProcessing();
								taskContext->appendMultiTaskProcessor(taskProcessor);
							}
						}


						context = taskContext;



					}
					return context;
				}



			}; // end namespace Calculation

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
