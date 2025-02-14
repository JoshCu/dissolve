// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "templates/listitem.h"
#include "templates/reflist.h"
#include "templates/vector3.h"
#include <memory>

// Forward Declarations
class AtomType;
class Configuration;
class CoreData;
class Isotopologue;
class LineParser;
class Module;
class ProcedureNode;
class Species;
class SpeciesSite;

// Keyword Base Class
class KeywordBase : public ListItem<KeywordBase>
{
    public:
    // Keyword Data Type
    enum KeywordDataType
    {
        AtomTypeRefListData,
        AtomTypeSelectionData,
        BoolData,
        ConfigurationVectorData,
        Data1DStoreData,
        Data2DStoreData,
        Data3DStoreData,
        DoubleData,
        DynamicSiteNodesData,
        ElementVectorData,
        EnumOptionsData,
        ExpressionData,
        ExpressionVariableVectorData,
        FileAndFormatData,
        Function1DData,
        GeometryListData,
        IntegerData,
        IsotopologueListData,
        IsotopologueSetData,
        LinkToKeywordData,
        ModuleData,
        ModuleGroupsData,
        ModuleRefListData,
        NodeData,
        NodeAndIntegerData,
        NodeBranchData,
        NodeValueData,
        NodeValueEnumOptionsData,
        NodeVectorData,
        ProcedureData,
        RangeData,
        SpeciesData,
        SpeciesSiteData,
        SpeciesSiteVectorData,
        SpeciesVectorData,
        StringData,
        ValueStoreData,
        Vec3DoubleData,
        Vec3IntegerData,
        Vec3NodeValueData,
        VectorIntegerDoubleData,
        VectorIntegerStringData,
        VectorStringPairData
    };
    KeywordBase(KeywordDataType type);
    virtual ~KeywordBase();
    // Return DataType name
    static std::string_view keywordDataType(KeywordDataType kdt);

    /*
     * Base Pointer Return
     */
    public:
    // Return base pointer for this (may be overloaded to provide access to other KeywordBase instance)
    virtual KeywordBase *base();

    /*
     * Keyword Description
     */
    public:
    // Keyword Options
    enum KeywordOption
    {
        NoOptions = 0,           /* Keyword has no options set */
        InRestartFileOption = 1, /* Keyword should have its data written to the restart file */
        ModificationRequiresSetUpOption =
            2 /* Modifying the keyword's data requires that the owning object requires setting up */
    };

    private:
    // Data type stored by keyword
    KeywordDataType type_;
    // Keyword name
    std::string name_;
    // Arguments string (for information)
    std::string arguments_;
    // Description of keyword, if any
    std::string description_;
    // Keyword option mask
    int optionMask_;

    protected:
    // Whether the current data value has ever been set
    bool set_;

    public:
    // Set name, description, arguments, and option mask
    void set(std::string_view name, std::string_view description, std::string_view arguments, int optionMask = NoOptions);
    // Flag that data has been set by some other means
    void setAsModified();
    // Return data type stored by keyword
    KeywordDataType type() const;
    // Return name of data type stored by keyword
    std::string_view typeName() const;
    // Return keyword name
    std::string_view name() const;
    // Return arguments string
    std::string_view arguments() const;
    // Return keyword description
    std::string_view description() const;
    // Return keyword option mask
    int optionMask() const;
    // Return whether specified option is set
    bool isOptionSet(KeywordOption opt) const;
    // Return if the current data object is empty
    virtual bool isDataEmpty() const;
    // Return whether the keyword has been set, and is not currently empty (if relevant)
    bool hasBeenSet() const;

    /*
     * Arguments
     */
    public:
    // Return minimum number of arguments accepted
    virtual int minArguments() const = 0;
    // Return maximum number of arguments accepted
    virtual int maxArguments() const = 0;
    // Check number of arguments provided to keyword
    bool validNArgs(int nArgsProvided) const;
    // Parse arguments from supplied LineParser, starting at given argument offset
    virtual bool read(LineParser &parser, int startArg, const CoreData &coreData) = 0;
    // Write keyword data to specified LineParser
    virtual bool write(LineParser &parser, std::string_view keywordName, std::string_view prefix = "") const = 0;

    /*
     * Parse Result
     */
    public:
    // Keyword Parse Result
    enum ParseResult
    {
        Unrecognised = -1,
        Failed = 0,
        Success = 1
    };

    /*
     * Conversion
     */
    public:
    // Return value (as bool)
    virtual bool asBool();
    // Return value (as int)
    virtual int asInt();
    // Return value (as double)
    virtual double asDouble();
    // Return value (as string)
    virtual std::string asString();
    // Return value as Vec3<int>
    virtual Vec3<int> asVec3Int();
    // Return value as Vec3<double>
    virtual Vec3<double> asVec3Double();

    /*
     * Object Management
     */
    private:
    // References to all keyword objects
    static RefList<KeywordBase> allKeywords_;

    protected:
    // Prune any references to the supplied AtomType in the contained data
    virtual void removeReferencesTo(std::shared_ptr<AtomType> at);
    // Prune any references to the supplied Configuration in the contained data
    virtual void removeReferencesTo(Configuration *cfg);
    // Prune any references to the supplied Isotopologue in the contained data
    virtual void removeReferencesTo(Isotopologue *iso);
    // Prune any references to the supplied Module in the contained data
    virtual void removeReferencesTo(Module *module);
    // Prune any references to the supplied Species in the contained data
    virtual void removeReferencesTo(Species *sp);
    // Prune any references to the supplied SpeciesSite in the contained data
    virtual void removeReferencesTo(SpeciesSite *spSite);
    // Prune any references to the supplied ProcedureNode in the contained data
    virtual void removeReferencesTo(ProcedureNode *node);

    public:
    // Gracefully deal with the specified object no longer being valid
    template <class O> static void objectNoLongerValid(O *object)
    {
        // Loop over all keyword objects and call their local functions
        for (auto kwd : allKeywords_)
            kwd->removeReferencesTo(object);
    }
};
