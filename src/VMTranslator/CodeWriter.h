#pragma once
#include <fstream>
#include <unordered_map>
#include <set>
#include <string>
#include "ECommandType.h"

/// <summary>
/// The module translates a parsed VM command into Hack assembly code.
/// </summary>
class CodeWriter
{
public:
    /// <summary>
    /// Opens the output file and gets ready to write into it
    /// </summary>
    /// <param name="filename">Name of the output file</param>
    CodeWriter(const std::string& filename, bool generateComment);

    /// <summary>
    /// Informs that the translation of a new VM file has started (called by VMTranslator).
    /// </summary>
    /// <param name="fileName">File name</param>
    void setFileName(std::string_view fileName);
    /// <summary>
    /// Writes to the output file the assembly code that implements the given arithmetic-logical command.
    /// </summary>
    /// <param name="command">Arithmetic-logical command</param>
    void writeArithmetic(const std::string& command);
    /// <summary>
    /// Writes to the output file the assembly code that implements the given push or pop command
    /// </summary>
    /// <param name="commandType">C_PUSH or C_POP</param>
    /// <param name="segment">Name of the segment</param>
    /// <param name="index">Numeric address</param>
    void writePushPop(ECommandType commandType, const std::string& segment, int index);
    /// <summary>
    /// Writes assembly code that effects the label command
    /// </summary>
    /// <param name="label">Label name</param>
    void writeLabel(const std::string& label);
    /// <summary>
    /// Writes assembly code that effects the goto command.
    /// </summary>
    /// <param name="label">Label name</param>
    void writeGoto(const std::string& label);
    /// <summary>
    /// Writes assembly code that effects the if-goto command.
    /// </summary>
    /// <param name="label">Label name</param>
    void writeIf(const std::string& label);
    /// <summary>
    /// Writes assembly code that effects the function command.
    /// </summary>
    /// <param name="functionName">Function name</param>
    /// <param name="nVars">Count of the function local variables</param>
    void writeFunction(const std::string& functionName, int nVars);
    /// <summary>
    /// Writes assembly code that effects the call command.
    /// </summary>
    /// <param name="functionName">Function name</param>
    /// <param name="nArgs">Count of the arguments that have been pushed onto the stack before the call</param>
    void writeCall(const std::string& functionName, int nArgs);
    /// <summary>
    /// Writes assembly code that effects the return command.
    /// </summary>
    void writeReturn();
    /// <summary>
    /// Validates if every label used in goto and if-goto is defined.
    /// Adjusts code at the end of the file.
    /// </summary>
    void writeFinalCode();

private:
    void initialCode();

    void writeComparisonCommand(const std::string& comparisonCheck);

    void writeLocalArgThisThat(ECommandType commandType, const std::string& segmentName, int index);
    void writePointer(ECommandType commandType, int index);
    void writeTemp(ECommandType commandType, int index);
    void writeConstant(ECommandType commandType, int index);
    void writeStatic(ECommandType commandType, int index);

    void setFunctionName(const std::string& functionName);
    std::string getFullLabelName(const std::string& label) const;

    std::string stack2DRegister() const;
    std::string DRegister2Stack() const;
    int getNumber(const std::string& label);

    std::unique_ptr<std::ofstream> mOutputFile;
    bool mGenerateComment = false;

    std::unordered_map<std::string, int> mCountersMap;
    bool mIsComparisonUsed = false;

    std::set<std::string, std::less<>> mDefinedLabels;
    std::set<std::string, std::less<>> mDefinedGoto;
    
    std::string mFileName = "";
    std::string mFunctionName = "";
    const std::string mDefaultFunctionName = "Main";
    bool mNoFunctionDefined = true;
};
