#include <iostream>
#include <cstdlib>
#include <windows.h> 
#include <ctime>
using namespace std;

int RandomNumber(int From, int To) {
	return rand() % (To - From + 1) + From;
}

enum enQuestionLevel { Easy = 1, Med = 2, Hard = 3, Mix = 4 };
enum enOperationType { Add = 1, Sub = 2, Mult = 3, Div = 4, MixOp = 5 };
enum enFinalResult { Pass = 1, Fail = 2 };
enum enAnswerResult { Right = 1, Wrong = 2 };

struct stQuestionInfo {
	int Numerator = 0;
	int Denominator = 0;
	char OperatorType;
	int Answer = 0;
};
struct stRoundInfo {
	short NumberOfQuestion = 0;
	enQuestionLevel QuestionLevel;
	enOperationType OperationType;
};
struct stGameResults {
	short NumberOfQuestion = 0;
	enOperationType OperationType;
	enQuestionLevel QuestionLevel;
	short NumberOfRightNumber = 0;
	short NumberOfWrongNumber;
	enFinalResult FinalResults;
};

char SpecifyOperatorType(enOperationType OperationType) {
	switch (OperationType) {
	case enOperationType::Add: return '+';
	case enOperationType::Sub: return '-';
	case enOperationType::Mult: return '*';
	case enOperationType::Div: return '/';
	case enOperationType::MixOp:
		switch (RandomNumber(1, 4)) {
		case enOperationType::Add: return '+';
		case enOperationType::Sub: return '-';
		case enOperationType::Mult: return '*';
		case enOperationType::Div: return '/';
		}
	}
	return '+';
}
int GenerateNumerator(enQuestionLevel QuestionLevel) {
	switch (QuestionLevel) {
	case enQuestionLevel::Easy: return RandomNumber(0, 10);
	case enQuestionLevel::Med: return RandomNumber(10, 50);
	case enQuestionLevel::Hard: return RandomNumber(50, 100);
	case enQuestionLevel::Mix: return RandomNumber(0, 100);
	}
	return 0;
}
int GenerateDenominator(enQuestionLevel QuestionLevel) {
	switch (QuestionLevel) {
	case enQuestionLevel::Easy: return RandomNumber(1, 10);
	case enQuestionLevel::Med: return RandomNumber(10, 50);
	case enQuestionLevel::Hard: return RandomNumber(50, 100);
	case enQuestionLevel::Mix: return RandomNumber(1, 100);
	}
	return 1;
}
void SetScreenColorDependingOnTheAnswer(enAnswerResult result) {
	switch (result) {
	case enAnswerResult::Right:
		system("color 2F"); break;
	case enAnswerResult::Wrong:
		system("color 4F"); cout << "\a"; break;
	}
}
void SetWinnerScreenColor(enFinalResult result) {
	switch (result) {
	case enFinalResult::Pass:
		system("color 2F"); break;
	case enFinalResult::Fail:
		system("color 4F"); cout << "\a"; break;
	}
}
int CalculateTheAnswer(int Numerator, int Denominator, char OperatorType) {
	switch (OperatorType) {
	case '+': return Numerator + Denominator;
	case '-': return Numerator - Denominator;
	case '*': return Numerator * Denominator;
	case '/': return Numerator / Denominator;
	}
	return 0;
}
stQuestionInfo GenerateQuestion(stRoundInfo RoundInfo) {
	stQuestionInfo QuestionInfo;
	QuestionInfo.Numerator = GenerateNumerator(RoundInfo.QuestionLevel);
	QuestionInfo.Denominator = GenerateDenominator(RoundInfo.QuestionLevel);
	QuestionInfo.OperatorType = SpecifyOperatorType(RoundInfo.OperationType);
	QuestionInfo.Answer = CalculateTheAnswer(QuestionInfo.Numerator, QuestionInfo.Denominator, QuestionInfo.OperatorType);
	cout << QuestionInfo.Numerator << endl;
	cout << QuestionInfo.Denominator << " " << QuestionInfo.OperatorType << endl;
	cout << "---------------------\n";
	return QuestionInfo;
}
enAnswerResult CheckAnswer(int RealAnswer, int PlayerAnswer) {
	if (RealAnswer == PlayerAnswer) {
		cout << "Right Answer :-)\n";
		return enAnswerResult::Right;
	}
	else {
		cout << "Wrong Answer :-(\n";
		cout << "The right answer is : " << RealAnswer << endl;
		return enAnswerResult::Wrong;
	}
}
string QuestionLevelName(enQuestionLevel Choice) {
	string arrGameChoices[4] = { "Easy", "Med", "Hard", "Mix" };
	return arrGameChoices[Choice - 1];
}
string OperationTypeName(enOperationType Choice) {
	string arrOpNames[5] = { "Add", "Sub", "Mult", "Div", "Mix" };
	return arrOpNames[Choice - 1];
}
void ShowFinalGameResults(stGameResults GameResults) {
	Sleep(1000);
	SetWinnerScreenColor(GameResults.FinalResults);
	system("chcp 437> nul");
	cout << string(30, char(196)) << endl;
	cout << (GameResults.FinalResults == enFinalResult::Pass ? "  [ Final Results is Pass ]\n" : "  [ Final Results is Fail ]\n");
	cout << string(30, char(196)) << endl;
	cout << "Number of Questions       : " << GameResults.NumberOfQuestion << "\n";
	cout << "Questions level           : " << QuestionLevelName(GameResults.QuestionLevel) << "\n";
	cout << "Operation Type            : " << OperationTypeName(GameResults.OperationType) << "\n";
	cout << "Number of Right Answers   : " << GameResults.NumberOfRightNumber << "\n";
	cout << "Number of Wrong Answers   : " << GameResults.NumberOfWrongNumber << "\n";
	cout << string(30, char(196)) << endl;
}
void ResetScreen() {
	system("color 07");
	system("cls");
}
short ReadNumberOfQuestion() {
	short NumberOfQuestion;
	do {
		cout << "How Many Questions do you want to answer ? ";
		cin >> NumberOfQuestion;
	} while (NumberOfQuestion <= 0);
	return NumberOfQuestion;
}
enQuestionLevel ReadQuestionLevel() {
	short QuestionLevel;
	do {
		cout << "Enter Questions Level [1] Easy , [2] Med , [3] Hard , [4] Mix ? ";
		cin >> QuestionLevel;
	} while (QuestionLevel < 1 || QuestionLevel > 4);
	return (enQuestionLevel)QuestionLevel;
}
enOperationType ReadOperationType() {
	short OperationType;
	do {
		cout << "Enter Operation Type [1] Add , [2] Sub , [3] Mult , [4] Div , [5] Mix ? ";
		cin >> OperationType;
	} while (OperationType < 1 || OperationType > 5);
	return (enOperationType)OperationType;
}
enFinalResult FinalResults(short NumberOfRightNumber, short NumberOfWrongNumber) {
	return (NumberOfRightNumber >= NumberOfWrongNumber) ? enFinalResult::Pass : enFinalResult::Fail;
}
stGameResults PlayGame(stRoundInfo& RoundInfo) {
	int PlayerAnswer;
	short NumberOfRightNumber = 0, NumberOfWrongNumber = 0;
	stGameResults GameResults;

	for (int i = 1; i <= RoundInfo.NumberOfQuestion; i++) {
		cout << "Question [" << i << "/" << RoundInfo.NumberOfQuestion << "]\n";
		stQuestionInfo question = GenerateQuestion(RoundInfo);
		cin >> PlayerAnswer;
		enAnswerResult Result = CheckAnswer(question.Answer, PlayerAnswer);
		SetScreenColorDependingOnTheAnswer(Result);
		if (Result == enAnswerResult::Right) NumberOfRightNumber++;
		else NumberOfWrongNumber++;
	}

	GameResults.NumberOfQuestion = RoundInfo.NumberOfQuestion;
	GameResults.NumberOfRightNumber = NumberOfRightNumber;
	GameResults.NumberOfWrongNumber = NumberOfWrongNumber;
	GameResults.QuestionLevel = RoundInfo.QuestionLevel;
	GameResults.OperationType = RoundInfo.OperationType; // fix
	GameResults.FinalResults = FinalResults(NumberOfRightNumber, NumberOfWrongNumber);
	return GameResults;
}
void StartGame() {
	char PlayAgain = 'y';
	stRoundInfo RoundInfo;
	stGameResults GameResults;
	do {
		ResetScreen();
		RoundInfo.NumberOfQuestion = ReadNumberOfQuestion();
		RoundInfo.QuestionLevel = ReadQuestionLevel();
		RoundInfo.OperationType = ReadOperationType();
		GameResults = PlayGame(RoundInfo);
		ShowFinalGameResults(GameResults);
		cout << "Do you want to play again ? Y/N? \n";
		cin >> PlayAgain;
	} while (PlayAgain == 'Y' || PlayAgain == 'y');
	cout << "\n                         Thank you for playing!\n";
}

int main() {
	srand((unsigned)time(NULL));
	system("chcp 437> nul");
	system("color 07");
	StartGame();
	return 0;
}