
/*!
 * \brief Class that handles the navigation and button presses.
 * \copyright MIT License
 * \author 97131004
 */
class navigation {
private:
	static int buttonState[4];
	static int buttonStatePrev[4];

public:
	static int create();
	static void buttonRisingEdge(const int& index, const int& pin, void(*buttonEvent)());
	static void button1Pressed();
	static void button2Pressed();
	static void button3Pressed();
	static void button4Pressed();
};
