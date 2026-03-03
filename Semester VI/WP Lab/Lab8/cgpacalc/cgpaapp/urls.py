from django.urls import path
from . import views

urlpatterns = [
    path('', views.page1, name='page1'),
    path('result/', views.page2, name='page2'),
]
