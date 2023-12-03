#pragma once
#include <fstream>
#include <string>
#include "EArithmetic.h"
#include "ESegment.h"

/// <summary>
/// VMWriter is responsible for writing VM commands into the output file.
/// </summary>
class VMWriter
{
public:
    /// <summary>
    /// Creates VMWriter.
    /// </summary>
    explicit VMWriter(std::shared_ptr<std::ofstream> outputFile);

    /// <summary>
    /// Writes a VM push command.
    /// </summary>
    void writePush(ESegment segment, int index);
    /// <summary>
    /// Writes a VM pop command.
    /// </summary>
    void writePop(ESegment segment, int index);
    /// <summary>
    /// Writes a VM arithmetic-logical command.
    /// </summary>
    void writeArithmetic(EArithmetic command);
    /// <summary>
    /// Writes a VM label command.
    /// </summary>
    void writeLabel(const std::string& label);
    /// <summary>
    /// Writes a VM goto command.
    /// </summary>
    void writeGoto(const std::string& label);
    /// <summary>
    /// Writes a VM if-goto command.
    /// </summary>
    void writeIf(const std::string& label);
    /// <summary>
    /// Writes a VM call command.
    /// </summary>
    void writeCall(const std::string& name, int nArgs);
    /// <summary>
    /// Writes a VM function command.
    /// </summary>
    void writeFunction(const std::string& name, int nArgs);
    /// <summary>
    /// Writes a VM return command.
    /// </summary>
    void writeReturn();

private:
    std::string segmentToString(ESegment segment) const;
    std::string arithmeticToString(EArithmetic segment) const;
    
    std::shared_ptr<std::ofstream> mOutputFile;
};
