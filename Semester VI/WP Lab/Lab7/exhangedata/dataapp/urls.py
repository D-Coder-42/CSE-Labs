from django.urls import path
from . import views

urlpatterns = [
    path('', views.first_page, name='details_input'),
    path('second/', views.second_page, name='details_show')
]