#pragma once

class ImageBlurrer;
class ImageParser;

struct SImage;

class MainMenu
{
public:
    MainMenu();
    ~MainMenu();

    void Menu();

private:
    ImageBlurrer* m_imageBlurrer;
    ImageParser* m_imageparser;

    SImage* image;
};

