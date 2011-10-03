/*
 *  PluginParameter.cpp
 *
 *  Created by David Rowland on 09/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

PluginParameter::PluginParameter()
{
	init("parameter",		// name
		 UnitGeneric,		// unit
		 "A parameter",		// description
		 1.0,				// value
		 0.0,				// min
		 1.0,				// max
		 0.0,				// default
		 1.0,				// skew factor
		 0.1,				// smooth coeff
		 0.01,				// step
		 String::empty);	// unit suffix
}

PluginParameter::PluginParameter (const PluginParameter& other)
{
	name = other.name;
    description = other.description;
    unitSuffix = other.unitSuffix;
    min = other.min;
    max = other.max;
    defaultValue = other.defaultValue;
	smoothCoeff = other.smoothCoeff;
    smoothValue = other.smoothValue;
	skewFactor = other.skewFactor;
    step = other.step;
	unit = other.unit;
    setValue (double (other.valueObject.getValue()));
}

void PluginParameter::init(const String& name_, ParameterUnit unit_, String description_,
                           double value_, double min_, double max_, double default_,
                           double skewFactor_, double smoothCoeff_, double step_, String unitSuffix_)
{
	name = name_;
	unit = unit_;
	description = description_;
	
	min = min_;
	max = max_;
	setValue (value_);
	defaultValue = default_;
	
	smoothCoeff = smoothCoeff_;
	smoothValue = getValue();
	
	skewFactor = skewFactor_;
	step = step_;
	
	unitSuffix = unitSuffix_;
	
	// default label suffix's, these can be changed later
	switch (unit)
	{
		case UnitPercent:       setUnitSuffix("%");                         break;
		case UnitSeconds:       setUnitSuffix("s");                         break;
		case UnitPhase:         setUnitSuffix(CharPointer_UTF8 ("°"));      break;
		case UnitHertz:         setUnitSuffix("Hz");                        break;
		case UnitDecibels:      setUnitSuffix("dB");                        break;
		case UnitDegrees:       setUnitSuffix(CharPointer_UTF8 ("°"));      break;
		case UnitMeters:        setUnitSuffix("m");                         break;
		case UnitBPM:           setUnitSuffix("BPM");                       break;
		case UnitMilliseconds:  setUnitSuffix("ms");                        break;
		default:                                                            break;
	}	
}

void PluginParameter::setValue(double value)
{
	valueObject = jlimit (min, max, value);
}

void PluginParameter::setNormalisedValue(double normalisedValue)
{
	setValue ((max - min) * jlimit (0.0, 1.0, normalisedValue) + min);
}

void PluginParameter::setUnitSuffix(String newSuffix)
{
	unitSuffix = newSuffix;
}

void PluginParameter::smooth()
{
	if (smoothValue != getValue())
	{
		if( (smoothCoeff == 1.0) || almostEqual (smoothValue, getValue()) )
			smoothValue = getValue(); 
		else
			smoothValue = ((getValue() - smoothValue) * smoothCoeff) + smoothValue;
	}
}

void PluginParameter::setSmoothCoeff (double newSmoothCoef)
{
	smoothCoeff = newSmoothCoef;
}

void PluginParameter::setSkewFactor (double newSkewFactor)
{
	skewFactor = newSkewFactor;
}

void PluginParameter::setSkewFactorFromMidPoint (const double valueToShowAtMidPoint)
{
	if (max > min)
        skewFactor = log (0.5) / log ((valueToShowAtMidPoint - min) / (max - min));
}

void PluginParameter::setStep (double newStep)
{
	step = newStep;
}

void PluginParameter::writeXml (XmlElement& xmlState)
{
	xmlState.setAttribute (name,	getValue());
}

void PluginParameter::readXml(const XmlElement* xmlState)
{
	setValue (xmlState->getDoubleAttribute (name, getValue()));
}

void PluginParameter::setupSlider(Slider &slider)
{
    slider.setRange             (min, max, step);
    slider.setSkewFactor        (skewFactor);
    slider.setValue             (getValue(), false);
    slider.setTextValueSuffix   (unitSuffix);
}

double PluginParameter::normaliseValue(double scaledValue)
{
	return ((scaledValue - min) / (max - min));
}

END_JUCE_NAMESPACE