#pragma once
#include <concepts>

// �R���Z�v�g���g���Đ����^�ƕ��������_���̂݃e���v���[�g�����ɓ�����悤��
template<class T>
concept ValueC = std::integral<T> || std::floating_point<T>;

