#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <windows.h>
#include <cstdlib>
#include <string.h>
#include <cmath>
#include <algorithm>

std::map<std::string, int> logicalOperandsWeights =
{{"=>", 1}, {"eqc", 2}, {"xor", 3}, {"or", 4}, {"and", 5}, {"not", 6}};


// split string connected by a spliter
std::vector<std::string> split(std::string expression, std::string spliter)
{
    std::vector<std::string> result;
    std::string token;
    size_t pos;
    while ((pos = expression.find(spliter)) != std::string::npos)
    {
        token = expression.substr(0, pos);
        result.push_back(token);
        expression.erase(0, pos + spliter.size());
    }
    token = expression.substr(0, pos);
    result.push_back(token);
    return result;
}

// change infix record to postfix record
std::string logicalInfixToONP(std::string infix)
{
    std::stack<std::string> operands;
    std::vector<std::string> splitedString = split(infix, " ");

    std::string result = "";

    for (int i = 0; i < splitedString.size(); i++)
    {
        if (splitedString[i] == "0" || splitedString[i] == "1")
            result = result + splitedString[i] + ' ';
        else if (splitedString[i] == "not" || splitedString[i] == "and" || splitedString[i] == "eqc" ||
                 splitedString[i] == "or" || splitedString[i] == "xor" || splitedString[i] == "=>")
        {
            while (!operands.empty() && logicalOperandsWeights[splitedString[i]] <= logicalOperandsWeights[operands.top()])
            {
                result = result + operands.top() + ' ';
                operands.pop();
            }
            operands.push(splitedString[i]);
        }
        else if (splitedString[i] == "(")
            operands.push(splitedString[i]);
        else if(splitedString[i] == ")")
        {
            while (operands.top() != "(")
            {
                result = result + operands.top() + ' ';
                operands.pop();
            }
            operands.pop();
        }
    }
    while (!operands.empty())
    {
        result = result + operands.top() + ' ';
        operands.pop();
    }
    return result;
}

// calculate postfix logic expression
bool calculateLogicalExpression(std::string logicalExpression)
{
    bool result = atoi(logicalExpression.c_str());
    logicalExpression = logicalExpression.substr(0, logicalExpression.size() - 1);
    std::vector<std::string> splitedExpression = split(logicalExpression, " ");
    std::stack<std::string> operands;


    for(int i=0; i<splitedExpression.size(); i++)
    {
        if (splitedExpression[i] == "0" || splitedExpression[i] == "1")
            operands.push(splitedExpression[i]);
        else if (splitedExpression[i] == "and" || splitedExpression[i] == "or" ||
                 splitedExpression[i] == "=>" || splitedExpression[i] == "xor")
        {
            bool first = atoi(operands.top().c_str());
            operands.pop();
            bool second = atoi(operands.top().c_str());
            operands.pop();

            if (splitedExpression[i] == "and")
                result = first && second;
            else if(splitedExpression[i] == "or")
                result = first || second;
            else if(splitedExpression[i] == "=>")
                result =  first || (!second);
            else if(splitedExpression[i] == "xor")
                result = !(first == second);

            operands.push(std::to_string(result));

        }
        else if (splitedExpression[i] == "not")
        {
            bool first = atoi(operands.top().c_str());
            operands.pop();

            result = !result;
            operands.push(std::to_string(!first));
        }
    }

    return result;
}


std::string Merge(std::vector<std::string> splitedExpression)
{
    std::string result = splitedExpression[0] + ' ';
    for (int i = 1; i < splitedExpression.size(); i++)
        result = result + splitedExpression[i] + ' ';

    return result;
}


void series(std::string expression)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    std::cout << "\n";
    std::cout << expression << std::endl << std::endl;

    std::map<char, int> letters;
    std::vector<std::string> splitedExpression = split(expression, " ");
    int index = 1;
    for(std::string element : splitedExpression)
        if (element.size() == 1 && element[0] >= 'a' && element[0] <= 'z')
        {
            if(letters[element[0]] == 0)
            {
                std::cout << element[0] << " ";
                letters[element[0]] = index;
                index = index + 1;
            }
        }

    std::cout << std::endl;
    for(int i = 0; i < std::pow(2, letters.size()); i++)
    {
        int number = i;
        std::vector<int> rests;
        std::vector<std::string> tempExpression = splitedExpression;
        for (int j = 0; j < letters.size(); j++)
        {
            rests.push_back(number%2);
            //std::cout << rests[j] << " ";
            number = number / 2;
        }
        std::reverse(rests.begin(), rests.end());
        for(int j=0; j<letters.size(); j++)
            std::cout << rests[j] << " ";
        for(int j=0; j<tempExpression.size(); j++)
        {
            if(tempExpression[j].size() == 1 && tempExpression[j][0] >= 'a' && tempExpression[j][0] <= 'z')
            {
                if(letters[tempExpression[j][0]] != 0)
                {
                    tempExpression[j] = std::to_string(rests[letters[tempExpression[j][0]] - 1]);
                }

            }
        }

        std::string result = Merge(tempExpression);
        std::cout << "      " << result << " ";

        if (calculateLogicalExpression(logicalInfixToONP(result)) == true)
        {
            SetConsoleTextAttribute(handle, 2);
            std::cout << "true" << std::endl;
        }
        else
        {
            SetConsoleTextAttribute(handle, 4);
            std::cout << "false" << std::endl;
        }

        SetConsoleTextAttribute(handle, 15);
    }

}

bool checkParentchases(std::string expression)
{
    std::stack<char> parentchases;
    for (int i = 0; i < expression.size(); i++)
    {
        if (expression[i] == '(') parentchases.push(expression[i]);
        else if (expression[i] == ')'){
            if(parentchases.empty() || parentchases.top() != '(')
                return false;
            else
                parentchases.pop();

        }
    }

    return parentchases.empty();
}

bool CheckCorrectnes(std::string expression)
{
    if (checkParentchases(expression) == false)
        throw 10;

    std::map<std::string, int>::iterator it;
    std::vector<std::string> splitedExpression = split(expression, " ");

    for (std::vector<std::string>::iterator it2 = splitedExpression.begin(); it2 != splitedExpression.end(); ++it2)
    {
        std::string operand = *it2;
        if(operand.size() > 1)
        {
            bool czy = false;
            for(it = logicalOperandsWeights.begin(); it != logicalOperandsWeights.end(); ++it)
            {
                if(it->first == operand)
                {
                    czy = true;
                    break;
                }
            }

            if(czy == false)
                throw ("Syntax error: no operand with name " + operand);
        }
    }

    return true;
}

std::string checkSpaces(std::string expression)
{
    std::string result = "";
    for(int i = 0; i < expression.size(); i++){
        if (expression[i] == '(' || expression[i] == ')')
            result = result + " " + expression[i] + " ";
        else result = result + expression[i];
    }

    std::vector<std::string> splited = split(result, " ");
    for(int i=0; i<splited.size(); i++)
        if (splited[i] == " ")
            splited[i] = "";

    return Merge(splited);

}


int main()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, 15);



    std::string expression;
    while(true)
    {
        bool isCorrect = true;
        std::cout << "enter infix expression: ";
        std::getline(std::cin, expression);

        //std::transform(expression.begin(), expression.end(), 0);

        try
        {
            CheckCorrectnes(expression);
        }
        catch(std::string ex)
        {
            SetConsoleTextAttribute(handle, 4);
            std::cout << ex << " \n\n";

            SetConsoleTextAttribute(handle, 15);
            system("pause");
            system("cls");
            isCorrect = false;
        }catch(int ex)
        {
            SetConsoleTextAttribute(handle, 4);
            std::cout << "Syntax Error: niedopowiednie nawiasy\n\n";

            SetConsoleTextAttribute(handle, 15);
            system("pause");
            system("cls");
            isCorrect = false;
        }

        if(isCorrect == true)
        {
            series(expression);

            std::cout << std::endl;

            system("pause");
            system("cls");
        }
    }






    //2 - zielony
    //4 - czerwony
    //15 - bialy



    return 0;
}

















/*
std::string checkCorecctness(std::string expression)
{
    std::string result = "";


    for(int i=0; i<expression.size(); i++)
    {
        if(expression[i] == ' ') continue;
        if(expression[i] == 'p' || expression[i] == 'q' || expression[i] == 'r')
            if(result.size() == 0) result = result + expression[i];
            if(result.size() > 0){
               if(result[result.size() - 1] != ' ') result = result + " " + expression[i] + " ";
               else result = result + expression[i] + " ";
            }
        else{
            result += expression[i];
        }
    }


    return result;
}
*/
