# 🧠 Akinator Clone (Command Line Edition)

This is a simple Akinator-style game that runs in the terminal. The program tries to guess the character you're thinking of by asking a series of yes/no questions.

---

## 🚀 Features

- Interactive command-line interface
- Answer from these 4 options (Yes/No/Prob Yes/Prob No)
- Simple Bayes reasoning to update the probabilities of characters
- Purely offline, no dependencies

---

## 🚀 How It Works

- The character data is stored in `characters.txt`, where every character is represented across **three lines**:
  - **Line 1:** Character name  
  - **Line 2:** List of traits/questions relevant to the character  
  - **Line 3:** Corresponding values (either `1` or `0`) indicating the presence or absence of each trait  

- Currently, only traits that the character **definitively possesses** are stored, so all values are `1` (binary representation of trait presence).

- This data is imported into a **hash map (dictionary)**:
  - **Key:** Character name  
  - **Value:** Another hash map mapping each trait/question to its value (`1` for "has trait", `0` otherwise)

- To determine the most informative question to ask next, the program computes the **variance** for each trait across all characters:
  - Let `T` be the total number of traits, and `C` be the number of characters.
  - For each trait, the program calculates the **Bernoulli variance**:  
    \[
    \text{Var}(X) = p(1 - p)
    \]
    where `p` is the proportion of characters that possess the trait.

- The algorithm selects the trait (i.e., question) with the **maximum variance**, as this splits the set of remaining possibilities most evenly.

- **Example:**  
  Suppose there are 4 characters — 2 male and 2 female. For the trait `is_male`,  
  \[
  p = \frac{2}{4} = 0.5,\quad 1 - p = 0.5 \Rightarrow \text{Var} = 0.25
  \]  
  This indicates a perfect 50/50 split, making it an optimal question.

-- After selecting a question and receiving a user response, the program updates the probability of each character being the correct one using **Bayes' Theorem**:

- `P(character | trait) = [P(trait | character) × P(character)] / P(trait)`

- Here:
- `P(character)` is the prior probability of the character (initially uniform)
- `P(trait | character)` is the likelihood of observing the user's answer given the character
- `P(trait)` is the total probability of that answer across all characters

- The likelihood `P(trait | character)` is calculated using a **likeness function**:

- `likeness = 1 - |userAnswer - traitValue|`

- If the user answers `1` (yes) and the trait value for the character is also `1`, then `likeness = 1`
- If the user answers `0` (no) and the trait value is `1`, then `likeness = 0`
- This is applied for every character to update their respective probabilities

- This approach ensures that with each new question, the character probabilities become more refined based on the user's responses.

- This approach is inspired by **Bayesian decision-making**, drawing from the method described in this article:  
  👉 [Building Akinator with Python Using Bayes’ Theorem](https://medium.com/analytics-vidhya/building-akinator-with-python-using-bayes-theorem-216253c98daa)

## 🛠️ Technologies Used

- C++
- Basic file I/O and hash maps

---

## 🛠️ Further Improvements

- Looking forward to add the functionality of adding new characters if guessed incorrectly or updating the original characters.

---

## 🛠️ Potential Drawbacks

- In this version, the bayes reasoning is quite accurate, however the issue is with the questions in the data set, if a character does not have a certain trait and the user answers yes to the question, then that character's probability goes to 0, it can be fixed by handling the values of user answers, for examples Yes = 1, No = 0, Prob Yes and Prob No equal 0.3. Changing No from 0 to 0.2 shows some difference but in some cases it fails. 

- The game seems to work perfeclty if the user answers each question accurately.

---
