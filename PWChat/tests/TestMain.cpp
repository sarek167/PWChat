#include <gtest/gtest.h>
#include <QApplication>
#include <QMetaType>


#include <iostream>

int main(int argc, char** argv)
{
    std::cerr << "🔥🔥🔥 MAIN FROM TESTMAIN 🔥🔥🔥" << std::endl;
    std::cerr.flush();

    QApplication app(argc, argv);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
