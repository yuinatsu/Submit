#pragma once
#include <concepts>

class ComponentBase;

// �R���|�[�l���g�̎擾�֐��p�̃R���Z�v�g
template<class T>
concept CompC =
	requires (T c) { T::id_; T::baseID_; }&&
std::derived_from<T, ComponentBase> ;
