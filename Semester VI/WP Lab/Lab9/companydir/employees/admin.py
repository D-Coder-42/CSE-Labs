from django.contrib import admin
from .models import Works, Lives

class WorksAdmin(admin.ModelAdmin):
    list_display = ('person_name', 'company_name', 'salary')

class LivesAdmin(admin.ModelAdmin):
    list_display = ('person_name', 'street', 'city')

admin.site.register(Works, WorksAdmin)
admin.site.register(Lives, LivesAdmin)
