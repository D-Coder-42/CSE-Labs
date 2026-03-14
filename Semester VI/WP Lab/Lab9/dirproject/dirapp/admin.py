from django.contrib import admin
from .models import Category, Page

class CategoryAdmin(admin.ModelAdmin):
    list_display = ('name', 'visits', 'likes')

class PageAdmin(admin.ModelAdmin):
    list_display = ('title', 'category', 'url', 'views')

admin.site.register(Category, CategoryAdmin)
admin.site.register(Page, PageAdmin)
