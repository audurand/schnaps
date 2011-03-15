/*
 * Testv2.cpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SCHNAPS/Plugins/Meds/Meds.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Meds;

/*!
 * \brief Default constructor.
 */
Testv2::Testv2() :
	Primitive(2),
	mLabel(""),
	mCost(NULL),
	mDiscountRate(NULL),
	mSensitivity(NULL),
	mSpecificity(NULL),
	mCost_Ref(""),
	mDiscountRate_Ref(""),
	mSensitivity_Ref(""),
	mSpecificity_Ref(""),
	mCostVariableLabel(""),
	mStateVariableLabel("")
{}

/*!
 * \brief Construct a medical test (second implementation) as a copy of an original.
 * \param inOriginal A const reference to the original medical test (second implementation).
 */
Testv2::Testv2(const Testv2& inOriginal) :
	Primitive(2),
	mLabel(inOriginal.mLabel.c_str()),
	mCost_Ref(inOriginal.mCost_Ref.c_str()),
	mDiscountRate_Ref(inOriginal.mDiscountRate_Ref.c_str()),
	mSensitivity_Ref(inOriginal.mSensitivity_Ref),
	mSpecificity_Ref(inOriginal.mSpecificity_Ref),
	mCostVariableLabel(inOriginal.mCostVariableLabel.c_str()),
	mStateVariableLabel(inOriginal.mStateVariableLabel.c_str())
{
	if (mCost_Ref.empty()) {
		mCost = Core::castHandleT<Core::Double>(inOriginal.mCost->clone());
	} else {
		mCost = Core::castHandleT<Core::Double>(inOriginal.mCost);
	}

	if (mDiscountRate_Ref.empty()) {
		mDiscountRate = Core::castHandleT<Core::Double>(inOriginal.mDiscountRate->clone());
	} else {
		mDiscountRate = Core::castHandleT<Core::Double>(inOriginal.mDiscountRate);
	}

	if (mSensitivity_Ref.empty()) {
		mSensitivity = Core::castHandleT<Core::Double>(inOriginal.mSensitivity->clone());
	} else {
		mSensitivity = Core::castHandleT<Core::Double>(inOriginal.mSensitivity);
	}

	if (mSpecificity_Ref.empty()) {
		mSpecificity = Core::castHandleT<Core::Double>(inOriginal.mSpecificity->clone());
	} else {
		mSpecificity = Core::castHandleT<Core::Double>(inOriginal.mSpecificity);
	}
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 * \throw SCHNAPS::Core::IOException if cost attribute and cost.ref attribute are missing.
 * \throw SCHNAPS::Core::IOException if discountRate attribute and discountRate.ref attribute are missing.
 * \throw SCHNAPS::Core::IOException if sensitivity attribute and sensitivity.ref attribute are missing.
 * \throw SCHNAPS::Core::IOException if specificity attribute and specificity.ref attribute are missing.
 * \throw SCHNAPS::Core::IOException if costVariableLabel attribute is missing.
 * \throw SCHNAPS::Core::IOException if stateVariableLabel attribute is missing.
 */
void Testv2::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != getName()) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

	// retrieve label
	if (inIter->getAttribute("label").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label expected!");
	} else {
		mLabel = inIter->getAttribute("label");
	}

	// retrieve cost
	if (inIter->getAttribute("cost").empty()) {
		if (inIter->getAttribute("cost.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "cost expected!");
		} else {
			mCost_Ref = inIter->getAttribute("cost.ref");
			std::stringstream lSS;
			lSS << "ref." << mCost_Ref;
			mCost = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
		}
	} else {
		mCost = new Core::Double(str2dbl(inIter->getAttribute("cost")));
	}

	// retrieve discount rate
	if (inIter->getAttribute("discountRate").empty()) {
		if (inIter->getAttribute("discountRate.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "discount rate expected!");
		} else {
			mDiscountRate_Ref = inIter->getAttribute("discountRate.ref");
			std::stringstream lSS;
			lSS << "ref." << mDiscountRate_Ref;
			mDiscountRate = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
		}
	} else {
		mDiscountRate = new Core::Double(str2dbl(inIter->getAttribute("discountRate")));
	}

	// retrieve sensitivity
	if (inIter->getAttribute("sensitivity").empty()) {
		if (inIter->getAttribute("sensitivity.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "sensitivity expected!");
		} else {
			mSensitivity_Ref = inIter->getAttribute("sensitivity.ref");
			std::stringstream lSS;
			lSS << "ref." << mSensitivity_Ref;
			mSensitivity = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
		}
	} else {
		mSensitivity = new Core::Double(str2dbl(inIter->getAttribute("sensitivity")));
	}

	// retrieve specificity
	if (inIter->getAttribute("specificity").empty()) {
		if (inIter->getAttribute("specificity.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "specificity expected!");
		} else {
			mSpecificity_Ref = inIter->getAttribute("specificity.ref");
			std::stringstream lSS;
			lSS << "ref." << mSpecificity_Ref;
			mSpecificity = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
		}
	} else {
		mSpecificity = new Core::Double(str2dbl(inIter->getAttribute("specificity")));
	}

	// retrieve label of cost variable
	if (inIter->getAttribute("costVariableLabel").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label of cost variable to refer expected!");
	}
	mCostVariableLabel = inIter->getAttribute("costVariableLabel");

	// retrieve label of state to test
	if (inIter->getAttribute("stateVariableLabel").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label of state variable to refer expected!");
	}
	mStateVariableLabel = inIter->getAttribute("stateVariableLabel");
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Testv2::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Testv2::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("label", mLabel);
	if (mCost_Ref.empty()) {
		ioStreamer.insertAttribute("cost.ref", "");
		ioStreamer.insertAttribute("cost", mCost->writeStr());
	} else {
		ioStreamer.insertAttribute("cost.ref", mCost_Ref);
	}
	if (mDiscountRate_Ref.empty()) {
		ioStreamer.insertAttribute("discountRate.ref", "");
		ioStreamer.insertAttribute("discountRate", mDiscountRate->writeStr());
	} else {
		ioStreamer.insertAttribute("discountRate.ref", mDiscountRate_Ref);
	}
	if (mSensitivity_Ref.empty()) {
		ioStreamer.insertAttribute("sensitivity.ref", "");
		ioStreamer.insertAttribute("sensitivity", mSensitivity->writeStr());
	} else {
		ioStreamer.insertAttribute("sensitivity.ref", mSensitivity_Ref);
	}
	if (mSpecificity_Ref.empty()) {
		ioStreamer.insertAttribute("specificity.ref", "");
		ioStreamer.insertAttribute("specificity", mSpecificity->writeStr());
	} else {
		ioStreamer.insertAttribute("specificity.ref", mSpecificity_Ref);
	}
	ioStreamer.insertAttribute("costVariableLabel", mCostVariableLabel);
	ioStreamer.insertAttribute("stateVariableLabel", mStateVariableLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Testv2::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if theis not defined for specific execution context.
 */
Core::AnyType::Handle Testv2::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (ioContext.getName() == "SimulationContext") {
		Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
		double lTime = lContext.getClock().getValue();

		// compute cost
		Core::Double lDenum(std::pow(mDiscountRate->getValue() + 1, lTime));
		Core::Double::Handle lCost = Core::castHandleT<Core::Double>(mCost->clone());
		lCost->div(lDenum);

		// set cost
		Core::Double::Handle lNewValue = Core::castHandleT<Core::Double>(lContext.getIndividual().getState().getVariableHandle(mCostVariableLabel)->clone());
		lNewValue->add(*lCost);
		lContext.getIndividual().getState().setVariable(mCostVariableLabel, lNewValue);

		// Apply test
		Core::Bool lState = Core::castObjectT<const Core::Bool&>(lContext.getIndividual().getState().getVariable(mStateVariableLabel));

		if (lState.getValue()) {
			// really positive
			if (lContext.getRandomizer().rollUniform() < mSensitivity->getValue()) {
				// tested positive
				getArgument(inIndex, 0, ioContext);
			} else {
				// tested negative
				getArgument(inIndex, 1, ioContext);
			}
		} else {
			// really negative
			if (lContext.getRandomizer().rollUniform() < mSpecificity->getValue()) {
				// tested negative
				getArgument(inIndex, 1, ioContext);
			} else {
				// tested positive
				getArgument(inIndex, 0, ioContext);
			}
		}
	} else {
		throw schnaps_RunTimeExceptionM("Primitive is not defined for context '" + ioContext.getName() + "'!");
	}
	return NULL;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Meds::Testv2::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& Testv2::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, 1);
	const static std::string lType("Any");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Testv2::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Testv2::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Testv2::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
