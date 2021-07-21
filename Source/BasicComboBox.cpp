#include "BasicComboBox.h"

BasicComboBox::BasicComboBox()
{

}

BasicComboBox::~BasicComboBox()
{

}

void BasicComboBox::setEqualizationCombo(int x_pos, int y_pos, int length, int height)
{
    this->setBounds(x_pos, y_pos, length, height);

    this->addItem("off", 1);
    this->addItem("on", 2);
}

void BasicComboBox::setWavesCombo(int x_pos, int y_pos, int length, int height)
{
    this -> setBounds(x_pos, y_pos, length, height);

    this->addItem("off", 1);
    this->addItem("chorus", 2);
    this->addItem("tap echo", 3);
    this->addItem("flanger", 4);
}

void BasicComboBox::setPresetCombo(int x_pos, int y_pos, int length, int height)
{
    this->setBounds(x_pos, y_pos, length, height);

    this->addItem("default", 1);
    this->addItem("jungle fade", 2);
    this->addItem("wide joe", 3);
    this->addItem("shadow", 4);
    this->addItem("repeter", 5);
    this->addItem("psycho", 6);
    this->addItem("mist", 7);
    this->addItem("chord", 8);
    this->addItem("disorder", 9);
    this->addItem("fighters", 10);
}

void BasicComboBox::setSteps(int x_pos, int y_pos, int length, int height)
{
    this->setBounds(x_pos, y_pos, length, height);

    this->addItem("1/1", 1);
    this->addItem("1/2", 2);
    this->addItem("1/4", 3);
    this->addItem("1/8", 4);
    this->addItem("1/16", 5);
    this->addItem("1/32", 6);

    this->addItem("1/1d", 7);
    this->addItem("1/2d", 8);
    this->addItem("1/4d", 9);
    this->addItem("1/8d", 10);
    this->addItem("1/16d", 11);
    this->addItem("1/32d", 12);

    this->addItem("1/1t", 13);
    this->addItem("1/2t", 14);
    this->addItem("1/4t", 15);
    this->addItem("1/8t", 16);
    this->addItem("1/16t", 17);
    this->addItem("1/32t", 18);
}
