#pragma once

/**
 * Usage example:
 *     using PropagateToEntities = NamedBool<class PropagateToEntitiesTag>;
 *     void ClearDirty(PropagateToEntities propagateToEntities_ = PropagateToEntities{ false });
 */
template <typename Name>
struct NamedBool
{
    explicit NamedBool(bool value_): _value{ value_ } {}

    // Delete non-bool constructors.
    template <typename T>
    NamedBool(T) = delete;

    operator bool() const { return _value; }
    NamedBool& operator= (bool value_) { _value = value_; return *this; }

private:
    bool _value;

};