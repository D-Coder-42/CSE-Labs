from django.urls import path
from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('category/<int:category_id>/', views.category_detail, name='category_detail'),
    path('category/<int:category_id>/like/', views.like_category, name='like_category'),
    path('page/<int:page_id>/view/', views.view_page, name='view_page'),
]
