# Best Practices

- Write fully documented code. Documentation is extremely important because no one wants to look at source code and waste time figuring out how to use a software. 
    - Comments tell why instead of what.
    - Describe temporary fixes with `@todo`.
- Write readable and easy to maintain code. 
> Any fool can write code that a computer can understand. Good programmers write code that humans can understand.
    - Don't repeat yourself (DRY). DRY means don't write the same code repeatedly. Duplicate code should be refactored into a single location (e.g file, function. etc), so it is reusable. It improves code maintainability.
    - Keep It Simple Stupid (KISS).
        - Simple code is less complex, less prone to bugs, and is easier to read and understand for you and the people who'll be working on the code in the future (including yourself).
        - Reinventing the wheel violates KISS. Before going on making your own solution, please try to find solutions for your problem on the Internet first. You don't want to solve problem that has been solved by others. 
    - Developers spend most of their productive time reading code, not writing it: debugging, checking commits done by others, learning new libraries, learning legacy code etc.
        - If your code can't be understand and is not readable, how come everyone can help you optimize it for performance or find any bug in it?
    - Poor code readability is:
        - Difficult to understand, thus it makes it hard for newcomers to edit the legacy code.
        - Hard to debug.
        - Hard to maintain.
        - Tricky to extend.
    - There are many ways how to improve our readability. Following are some tips on how to improve our readability:
        - Consistency. The more consistent the code base is in naming, formatting, structure, etc., the easier it is to jump into arbitrary code and understand it.
        - Good variable names.
    - Do incremental refactoring to improve code maintainability.
- Write tests. By having tests, you make sure all bugs are caught and fixed.
- Statically link.
    - The complexities to run your application get higher as dependencies needed by your application get larger.
    - There is no guarantee that dependencies needed by your application presents in the environment where it deployed. This increases the complexity in the deployment.
    - Statically link solves this by including all dependencies into one binary file and it can be run on targeted OS even without the absence of the dependencies.
    - It decreases the complexity thus it increases the quality of the application.
- No hard coding.