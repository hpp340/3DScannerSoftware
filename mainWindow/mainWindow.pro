#-------------------------------------------------
#
# Project created by QtCreator 2014-07-16T14:26:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

qtHaveModule(opengl)
{
    QT  += opengl

    TARGET = mainWindow
    TEMPLATE = app


    SOURCES += main.cpp\
            mainwindow.cpp \
        showmesh.cpp \
    PlyCloud.cpp

    HEADERS  += mainwindow.h \
        showmesh.h \
    headers/PlyCloud.h \
    headers/Point.h \
    ui_mainwindow.h \
    headers/glm/detail/_features.hpp \
    headers/glm/detail/_fixes.hpp \
    headers/glm/detail/_literals.hpp \
    headers/glm/detail/_noise.hpp \
    headers/glm/detail/_swizzle.hpp \
    headers/glm/detail/_swizzle_func.hpp \
    headers/glm/detail/_vectorize.hpp \
    headers/glm/detail/func_common.hpp \
    headers/glm/detail/func_exponential.hpp \
    headers/glm/detail/func_geometric.hpp \
    headers/glm/detail/func_integer.hpp \
    headers/glm/detail/func_matrix.hpp \
    headers/glm/detail/func_noise.hpp \
    headers/glm/detail/func_packing.hpp \
    headers/glm/detail/func_trigonometric.hpp \
    headers/glm/detail/func_vector_relational.hpp \
    headers/glm/detail/hint.hpp \
    headers/glm/detail/intrinsic_common.hpp \
    headers/glm/detail/intrinsic_exponential.hpp \
    headers/glm/detail/intrinsic_geometric.hpp \
    headers/glm/detail/intrinsic_integer.hpp \
    headers/glm/detail/intrinsic_matrix.hpp \
    headers/glm/detail/intrinsic_trigonometric.hpp \
    headers/glm/detail/intrinsic_vector_relational.hpp \
    headers/glm/detail/precision.hpp \
    headers/glm/detail/setup.hpp \
    headers/glm/detail/type_float.hpp \
    headers/glm/detail/type_gentype.hpp \
    headers/glm/detail/type_half.hpp \
    headers/glm/detail/type_int.hpp \
    headers/glm/detail/type_mat.hpp \
    headers/glm/detail/type_mat2x2.hpp \
    headers/glm/detail/type_mat2x3.hpp \
    headers/glm/detail/type_mat2x4.hpp \
    headers/glm/detail/type_mat3x2.hpp \
    headers/glm/detail/type_mat3x3.hpp \
    headers/glm/detail/type_mat3x4.hpp \
    headers/glm/detail/type_mat4x2.hpp \
    headers/glm/detail/type_mat4x3.hpp \
    headers/glm/detail/type_mat4x4.hpp \
    headers/glm/detail/type_vec.hpp \
    headers/glm/detail/type_vec1.hpp \
    headers/glm/detail/type_vec2.hpp \
    headers/glm/detail/type_vec3.hpp \
    headers/glm/detail/type_vec4.hpp \
    headers/glm/gtc/constants.hpp \
    headers/glm/gtc/epsilon.hpp \
    headers/glm/gtc/matrix_access.hpp \
    headers/glm/gtc/matrix_integer.hpp \
    headers/glm/gtc/matrix_inverse.hpp \
    headers/glm/gtc/matrix_transform.hpp \
    headers/glm/gtc/noise.hpp \
    headers/glm/gtc/packing.hpp \
    headers/glm/gtc/quaternion.hpp \
    headers/glm/gtc/random.hpp \
    headers/glm/gtc/reciprocal.hpp \
    headers/glm/gtc/type_precision.hpp \
    headers/glm/gtc/type_ptr.hpp \
    headers/glm/gtc/ulp.hpp \
    headers/glm/gtx/associated_min_max.hpp \
    headers/glm/gtx/bit.hpp \
    headers/glm/gtx/closest_point.hpp \
    headers/glm/gtx/color_space.hpp \
    headers/glm/gtx/color_space_YCoCg.hpp \
    headers/glm/gtx/compatibility.hpp \
    headers/glm/gtx/component_wise.hpp \
    headers/glm/gtx/constants.hpp \
    headers/glm/gtx/dual_quaternion.hpp \
    headers/glm/gtx/epsilon.hpp \
    headers/glm/gtx/euler_angles.hpp \
    headers/glm/gtx/extend.hpp \
    headers/glm/gtx/extented_min_max.hpp \
    headers/glm/gtx/fast_exponential.hpp \
    headers/glm/gtx/fast_square_root.hpp \
    headers/glm/gtx/fast_trigonometry.hpp \
    headers/glm/gtx/gradient_paint.hpp \
    headers/glm/gtx/handed_coordinate_space.hpp \
    headers/glm/gtx/inertia.hpp \
    headers/glm/gtx/int_10_10_10_2.hpp \
    headers/glm/gtx/integer.hpp \
    headers/glm/gtx/intersect.hpp \
    headers/glm/gtx/io.hpp \
    headers/glm/gtx/log_base.hpp \
    headers/glm/gtx/matrix_cross_product.hpp \
    headers/glm/gtx/matrix_interpolation.hpp \
    headers/glm/gtx/matrix_major_storage.hpp \
    headers/glm/gtx/matrix_operation.hpp \
    headers/glm/gtx/matrix_query.hpp \
    headers/glm/gtx/matrix_transform_2d.hpp \
    headers/glm/gtx/mixed_product.hpp \
    headers/glm/gtx/multiple.hpp \
    headers/glm/gtx/noise.hpp \
    headers/glm/gtx/norm.hpp \
    headers/glm/gtx/normal.hpp \
    headers/glm/gtx/normalize_dot.hpp \
    headers/glm/gtx/number_precision.hpp \
    headers/glm/gtx/optimum_pow.hpp \
    headers/glm/gtx/orthonormalize.hpp \
    headers/glm/gtx/perpendicular.hpp \
    headers/glm/gtx/polar_coordinates.hpp \
    headers/glm/gtx/projection.hpp \
    headers/glm/gtx/quaternion.hpp \
    headers/glm/gtx/random.hpp \
    headers/glm/gtx/raw_data.hpp \
    headers/glm/gtx/reciprocal.hpp \
    headers/glm/gtx/rotate_normalized_axis.hpp \
    headers/glm/gtx/rotate_vector.hpp \
    headers/glm/gtx/scalar_relational.hpp \
    headers/glm/gtx/simd_mat4.hpp \
    headers/glm/gtx/simd_quat.hpp \
    headers/glm/gtx/simd_vec4.hpp \
    headers/glm/gtx/spline.hpp \
    headers/glm/gtx/std_based_type.hpp \
    headers/glm/gtx/string_cast.hpp \
    headers/glm/gtx/transform.hpp \
    headers/glm/gtx/transform2.hpp \
    headers/glm/gtx/ulp.hpp \
    headers/glm/gtx/unsigned_int.hpp \
    headers/glm/gtx/vec1.hpp \
    headers/glm/gtx/vector_angle.hpp \
    headers/glm/gtx/vector_query.hpp \
    headers/glm/gtx/wrap.hpp \
    headers/glm/common.hpp \
    headers/glm/exponential.hpp \
    headers/glm/ext.hpp \
    headers/glm/fwd.hpp \
    headers/glm/geometric.hpp \
    headers/glm/glm.hpp \
    headers/glm/integer.hpp \
    headers/glm/mat2x2.hpp \
    headers/glm/mat2x3.hpp \
    headers/glm/mat2x4.hpp \
    headers/glm/mat3x2.hpp \
    headers/glm/mat3x3.hpp \
    headers/glm/mat3x4.hpp \
    headers/glm/mat4x2.hpp \
    headers/glm/mat4x3.hpp \
    headers/glm/mat4x4.hpp \
    headers/glm/matrix.hpp \
    headers/glm/packing.hpp \
    headers/glm/trigonometric.hpp \
    headers/glm/vec2.hpp \
    headers/glm/vec3.hpp \
    headers/glm/vec4.hpp \
    headers/glm/vector_relational.hpp

    FORMS    += mainwindow.ui

    RESOURCES += \
        menu.qrc

    win32 {
    LIBS += "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64\GlU32.Lib" \
        "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64\OpenGL32.Lib"
    }
}
